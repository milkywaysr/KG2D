#include "KGDirectX11Shader.h"
#include <string>
#include <vector>
#include "WICTextureLoader/WICTextureLoader.h"
#include "KG2DTool.h"
#include "KGShaderObject.h"
#include "KGSprite.h"
#include "KGLabel.h"
using namespace std;

#define KG_SHOW_FPS

struct KGVertex
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
};

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};

struct CBMatrix
{
	XMMATRIX mMatrix;
};

//纹理缓存元素
struct DirectX11Texture
{
	ID3D11ShaderResourceView *texture;
	KGSize size;
};

KGDirectX11Shader::KGDirectX11Shader(HWND hwnd)
{
	//初始化渲染模块
	mHwnd = hwnd;
	HRESULT hr = S_OK;

	////Create device and context
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//定义交换链
	RECT rc;
	GetClientRect(mHwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = mHwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;

	//创建设备和交换链
	D3D_FEATURE_LEVEL levels[] = { 
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, levels, 2, D3D11_SDK_VERSION, &sd, &mSwapChain, &mDevice, &mFeatureLevel, &mImmediateContext);
	HR(hr, "Create SwapChain and Device Faild");

	//create renderTargetView
	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer));
	hr = mDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	HR(hr, "backBuffer");
	backBuffer->Release();

	//创建深度缓存
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = mDevice->CreateTexture2D(&descDepth, NULL, &mDepthStencil);
	HR(hr, "descDepth");

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = mDevice->CreateDepthStencilView(mDepthStencil, &descDSV, &mDepthStencilView);
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	HR(hr, "descDSV");

	//设置视窗
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<FLOAT>(width);
	mScreenViewport.Height = static_cast<FLOAT>(height);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mImmediateContext->RSSetViewports(1, &mScreenViewport);

	//设定各种矩阵
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mScale, I);
	XMStoreFloat4x4(&mWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mAnchor, I);
	XMStoreFloat4x4(&mRotation, I);
	//投影矩阵
	XMMATRIX P = XMMatrixOrthographicLH(width, height, 0.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);

	CreateFX();
	CreateBuffer();
}


KGDirectX11Shader::~KGDirectX11Shader()
{
	//释放DirectX11的COM
	mCBRotation->Release();
	mCBAnchor->Release();
	mCBView->Release();
	mCBWorld->Release();
	mCBScale->Release();
	mCBProjection->Release();

	for (auto &tmp : mTextureBuffer)
	{
		if (tmp.second.texture)
			tmp.second.texture->Release();
	}
	mVertexLayout->Release();
	mSamplerLinear->Release();

	mPS->Release();
	mVS->Release();
	mBoxIB->Release();
	mBoxVB->Release();

	mBlendState->Release();
	mDepthStencilView->Release();
	mDepthStencil->Release();

	if (mImmediateContext) mImmediateContext->ClearState();
	if (mRenderTargetView) mRenderTargetView->Release();
	if (mSwapChain) mSwapChain->Release();
	if (mImmediateContext) mImmediateContext->Release();

	//这段代码用来检测是否有没有释放的COM
	/*ID3D11Debug* pD3dDebug;
	mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pD3dDebug));
	pD3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);*/
	//-------------------------------------
	if (mDevice) mDevice->Release();
}

void KGDirectX11Shader::Render(const list<KGShaderObject*>& list, const KGCamera& camera)
{
	//清除渲染缓存和深度缓存
	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, ClearColor);
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//设置观察矩阵
	KGPoint viewPoint = camera.GetCenterPoint();
	XMVECTOR pos = XMVectorSet(viewPoint.x, viewPoint.y, -5.0f, 1.0f);
	XMVECTOR target = XMVectorSet(viewPoint.x, viewPoint.y, 0.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	CBMatrix cbView;
	cbView.mMatrix = XMMatrixTranspose(V);
	mImmediateContext->UpdateSubresource(mCBView, 0, NULL, &cbView, 0, 0);

	vector<KGLabel*> labelList;
	//渲染精灵
	for (auto &tmp : list)
	{
		if (tmp->IsVisible())
		{
			if (typeid(*tmp) == typeid(KGSprite))
			{
				//渲染精灵
				KGSprite *pSprite = dynamic_cast<KGSprite*>(tmp);

				//设置纹理
				KGTexture texture = pSprite->GetTexture();
				DirectX11Texture dx11Texture = mTextureBuffer[texture.GetTextureName()];
				//没有这个纹理,跳过这个精灵
				if (dx11Texture.texture == nullptr)
				{
					mTextureBuffer.erase(texture.GetTextureName());
					continue;
				}
				mImmediateContext->PSSetShaderResources(0, 1, &dx11Texture.texture);

				//设置锚点矩阵
				KGPoint anchorPoint = pSprite->GetAnchor();
				XMMATRIX A =
					XMMatrixTranslation(
						-anchorPoint.x,
						-anchorPoint.y, 0.0f);
				CBMatrix cbAnchor;
				cbAnchor.mMatrix = XMMatrixTranspose(A);
				mImmediateContext->UpdateSubresource(mCBAnchor, 0, NULL, &cbAnchor, 0, 0);

				//设置缩放矩阵
				KGSize scale = pSprite->GetSize();
				XMMATRIX S =
					XMMatrixScaling(scale.width, scale.higth, 1.0f);
				CBMatrix cbScale;
				cbScale.mMatrix = XMMatrixTranspose(S);
				mImmediateContext->UpdateSubresource(mCBScale, 0, NULL, &cbScale, 0, 0);

				//设置旋转矩阵
				float rotation = pSprite->GetRotation();
				XMMATRIX R =
					XMMatrixRotationZ(rotation);
				CBMatrix cbRotation;
				cbRotation.mMatrix = XMMatrixTranspose(R);
				mImmediateContext->UpdateSubresource(mCBRotation, 0, NULL, &cbRotation, 0, 0);

				//设置世界矩阵
				KGPoint position = pSprite->GetPosition();
				float z = pSprite->GetZOrder();
				XMMATRIX W =
					XMMatrixTranslation(
						position.x,
						position.y,
						z);
				CBMatrix cbWorld;
				cbWorld.mMatrix = XMMatrixTranspose(W);
				mImmediateContext->UpdateSubresource(mCBWorld, 0, NULL, &cbWorld, 0, 0);

				mImmediateContext->VSSetConstantBuffers(0, 1, &mCBView);
				mImmediateContext->VSSetConstantBuffers(1, 1, &mCBProjection);
				mImmediateContext->VSSetConstantBuffers(2, 1, &mCBWorld);
				mImmediateContext->VSSetConstantBuffers(3, 1, &mCBScale);
				mImmediateContext->VSSetConstantBuffers(4, 1, &mCBAnchor);
				mImmediateContext->VSSetConstantBuffers(5, 1, &mCBRotation);
				//绘制精灵
				mImmediateContext->DrawIndexed(6, 0, 0);
			}
			else
			{
				labelList.push_back(dynamic_cast<KGLabel*>(tmp));
			}
		}
	}

#ifndef KG_SHOW_FPS
	if (!labelList.empty())
	{
#endif
		HDC hdc;
		IDXGISurface1* surface1 = NULL;
		HRESULT hr = mSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&surface1);
		if (SUCCEEDED(hr))
		{
			hr = surface1->GetDC(FALSE, &hdc);
			if (SUCCEEDED(hr))
			{
				//设置字体大小、颜色
				SetBkMode(hdc, TRANSPARENT);
				SetTextColor(hdc, RGB(255, 255, 255));

				HFONT font;
				LOGFONT lFont;
				memset(&lFont, 0, sizeof(lFont));
				lFont.lfWeight = 700;
				

#ifdef KG_SHOW_FPS
				//绘制fps
				lFont.lfHeight = 20;
				font = CreateFontIndirect(&lFont);
				HGDIOBJ old = SelectObject(hdc, font);
				DeleteObject(old);

				std::wstring fps_str = std::to_wstring(g_fps);
				RECT rect = { 0,0,100,100 };
				DrawText(hdc, fps_str.c_str(), -1, &rect, DT_LEFT);
#endif

				for (auto tmp : labelList)
				{
					const KGColorRGB &labelColor = tmp->GetColor();
					SetTextColor(hdc, RGB(labelColor.red, labelColor.green, labelColor.blue));

					lFont.lfHeight = tmp->GetFontSize();
					font = CreateFontIndirect(&lFont);
					HGDIOBJ old = SelectObject(hdc, font);
					DeleteObject(old);

					RECT r = { tmp->GetPosition().x, tmp->GetPosition().y,
						tmp->GetPosition().x + tmp->GetLabelSize().width, 
						tmp->GetPosition().y + tmp->GetLabelSize().higth };

					DrawText(hdc, tmp->GetLabel().c_str(), -1, &r, DT_LEFT | DT_WORDBREAK);
				}
				DeleteObject(font);
				surface1->ReleaseDC(NULL);
			}
			surface1->Release();
			mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
		}
#ifndef KG_SHOW_FPS
	}
#endif
	mSwapChain->Present(0, 0);
}

KGTexture KGDirectX11Shader::CreateTextureWithFile(wstring &fileName)
{
	wstring textureName;
	KGSize textureSize;
	KGTexture ret;
	//先判断纹理在不在缓存中
	if (mTextureBuffer.find(fileName) != mTextureBuffer.end())
	{
		DirectX11Texture tex = mTextureBuffer[fileName];
		textureName = fileName;
		textureSize = tex.size;
	}
	else
	{
		DirectX11Texture dx11tex;
		ID3D11Texture2D * tx;
		//加载纹理
		HRESULT hr;
		hr = DirectX::CreateWICTextureFromFile(mDevice, fileName.c_str(), (ID3D11Resource**)&tx, &dx11tex.texture, 0);
		HR(hr, "纹理创建失败");
		
		//读取纹理大小
		D3D11_TEXTURE2D_DESC txd;
		tx->GetDesc(&txd);
		tx->Release();
		dx11tex.size.width = txd.Width;
		dx11tex.size.higth = txd.Height;

		//将纹理加入buffer
		mTextureBuffer[fileName] = dx11tex;

		textureName = fileName;
		textureSize = dx11tex.size;
	}
	ret.SetTextureData(textureName, textureSize);
	return ret;
}

KGSize KGDirectX11Shader::CalculateLabelSize(wstring & label, float fontSize, KGSize labelSize)
{
	HDC hdc;
	RECT rect = {0, 0, 0, 0};
	if (labelSize.width <= 0)
		rect.right = 800;
	else
		rect.right = labelSize.width;

	if (labelSize.higth <= 0)
		rect.bottom = 600;
	else
		rect.bottom = labelSize.higth;

	IDXGISurface1* surface1 = NULL;
	HRESULT hr = mSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&surface1);
	if (SUCCEEDED(hr))
	{
		hr = surface1->GetDC(TRUE, &hdc);
		if (SUCCEEDED(hr))
		{
			//设置字体大小、颜色
			HFONT font;
			LOGFONT lFont;
			memset(&lFont, 0, sizeof(lFont));
			lFont.lfHeight = fontSize;
			lFont.lfWeight = 700;
			font = CreateFontIndirect(&lFont);
			HGDIOBJ old = SelectObject(hdc, font);
			DeleteObject(old);
			DrawText(hdc, label.c_str(), -1, &rect, DT_LEFT | DT_CALCRECT | DT_WORDBREAK);
			DeleteObject(font);
			surface1->ReleaseDC(NULL);
		}
		surface1->Release();
	}
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	return KGSize(rect.right - rect.left, rect.bottom - rect.top);
}

void KGDirectX11Shader::CreateBuffer()
{
	HRESULT hr;
	//创建顶点buff
	KGVertex vertices[] =
	{
		{ XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(KGVertex) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	hr = mDevice->CreateBuffer(&vbd, &vinitData, &mBoxVB);
	HR(hr, "vbd");

	UINT indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	hr = mDevice->CreateBuffer(&ibd, &iinitData, &mBoxIB);
	HR(hr, "ibd");

	UINT stride = sizeof(KGVertex);
	UINT offset = 0;
	mImmediateContext->IASetVertexBuffers(0, 1, &mBoxVB, &stride, &offset);
	mImmediateContext->IASetIndexBuffer(mBoxIB, DXGI_FORMAT_R32_UINT, 0);

	mImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//创建纹理状态
	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = mDevice->CreateSamplerState(&sampDesc, &mSamplerLinear);
	HR(hr, "sampDesc");
	mImmediateContext->PSSetSamplers(0, 1, &mSamplerLinear);
	
	//初始化CB
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(CBMatrix);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;

	//观察矩阵
	hr = mDevice->CreateBuffer(&cbd, NULL, &mCBView);
	HR(hr, "cbv");
	CBMatrix cbv;
	cbv.mMatrix = XMMatrixTranspose(XMLoadFloat4x4(&mView));
	mImmediateContext->UpdateSubresource(mCBView, 0, NULL, &cbv, 0, 0);

	//投影矩阵
	hr = mDevice->CreateBuffer(&cbd, NULL, &mCBProjection);
	HR(hr, "cbp");
	CBMatrix cbp;
	cbp.mMatrix = XMMatrixTranspose(XMLoadFloat4x4(&mProj));
	mImmediateContext->UpdateSubresource(mCBProjection, 0, NULL, &cbp, 0, 0);

	//世界矩阵
	hr = mDevice->CreateBuffer(&cbd, NULL, &mCBWorld);
	HR(hr, "cbw");
	CBMatrix cbw;
	cbw.mMatrix = XMMatrixTranspose(XMLoadFloat4x4(&mWorld));
	mImmediateContext->UpdateSubresource(mCBWorld, 0, NULL, &cbw, 0, 0);
	//缩放矩阵
	hr = mDevice->CreateBuffer(&cbd, NULL, &mCBScale);
	HR(hr, "cbw");
	CBMatrix cbs;
	cbs.mMatrix = XMMatrixTranspose(XMLoadFloat4x4(&mScale));
	mImmediateContext->UpdateSubresource(mCBScale, 0, NULL, &cbs, 0, 0);
	//锚点矩阵，其实就是位移一下
	hr = mDevice->CreateBuffer(&cbd, NULL, &mCBAnchor);
	HR(hr, "cba");
	CBMatrix cba;
	cba.mMatrix = XMMatrixTranspose(XMLoadFloat4x4(&mAnchor));
	mImmediateContext->UpdateSubresource(mCBAnchor, 0, NULL, &cba, 0, 0);
	//旋转矩阵
	hr = mDevice->CreateBuffer(&cbd, NULL, &mCBRotation);
	HR(hr, "cba");
	CBMatrix cbr;
	cbr.mMatrix = XMMatrixTranspose(XMLoadFloat4x4(&mRotation));
	mImmediateContext->UpdateSubresource(mCBRotation, 0, NULL, &cba, 0, 0);
}

void KGDirectX11Shader::CreateFX()
{
	HRESULT hr;

	ID3DBlob* pVSBlob = 0;
	ID3DBlob* pMsgs = 0;
	hr = D3DCompileFromFile(L"fxfile.fx", NULL, NULL, "VS", "vs_5_0", 0, 0, &pVSBlob, &pMsgs);
	if (FAILED(hr))
	{
		if (pMsgs != NULL)
			OutputDebugStringA((char*)pMsgs->GetBufferPointer());
	}

	hr = mDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &mVS);
	HR(hr, "VS");
	mImmediateContext->VSSetShader(mVS, NULL, 0);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = mDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &mVertexLayout);
	HR(hr, "InputLayout");
		mImmediateContext->IASetInputLayout(mVertexLayout);
	pVSBlob->Release();

	ID3DBlob *pPSBolb = NULL;
	hr = D3DCompileFromFile(L"fxfile.fx", NULL, NULL, "PS", "ps_5_0", 0, 0, &pPSBolb, &pMsgs);
	if (FAILED(hr))
	{
		if (pMsgs != NULL)
			OutputDebugStringA((char*)pMsgs->GetBufferPointer());
	}
	hr = mDevice->CreatePixelShader(pPSBolb->GetBufferPointer(), pPSBolb->GetBufferSize(), NULL, &mPS);
	HR(hr, "PS");
	mImmediateContext->PSSetShader(mPS, NULL, 0);
	pPSBolb->Release();

	//设置混合
	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = mDevice->CreateBlendState(&blendDesc, &mBlendState);
	HR(hr, "Falid Blend");

	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	mImmediateContext->OMSetBlendState(mBlendState, NULL, 0xffffffff);
}