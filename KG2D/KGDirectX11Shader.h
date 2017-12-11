#pragma once
#include <map>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "d3dx11effect.h"
#include "KGShaderInterface.h"
#include "KGTexture.h"
using namespace std;
using namespace DirectX;

struct DirectX11Texture;

class KGDirectX11Shader :
	public KGShaderInterface
{
public:
	KGDirectX11Shader(HWND hwnd);
	~KGDirectX11Shader();

	virtual void Render(const list<KGShaderObject*>&, const KGCamera&);
	virtual KGTexture CreateTextureWithFile(wstring &fileName);
	virtual KGSize CalculateLabelSize(wstring &label, float , KGSize);
	
	void CreateBuffer();
	void CreateFX();
private:
	HWND				mHwnd;					//���ھ��
	D3D_DRIVER_TYPE		mDriverType;			//�豸����
	D3D_FEATURE_LEVEL	mFeatureLevel;			
	ID3D11Device		*mDevice;				//Direct�豸
	ID3D11DeviceContext *mImmediateContext;		//�豸������
	IDXGISwapChain		*mSwapChain;			//������
	ID3D11RenderTargetView *mRenderTargetView;	//��ȾĿ����ͼ
	ID3D11Texture2D *mDepthStencil;				//��Ȼ���
	ID3D11DepthStencilView *mDepthStencilView;	//��Ȼ�����ͼ
	D3D11_VIEWPORT mScreenViewport;				//��ͼ
	ID3D11BlendState *mBlendState;				//���״̬��͸��Ч����

	//���㻺��
	ID3D11Buffer* mBoxVB;
	ID3D11Buffer* mBoxIB;
	ID3D11VertexShader *mVS;
	ID3D11PixelShader *mPS;
	ID3D11InputLayout *mVertexLayout;

	//����
	ID3D11SamplerState *mSamplerLinear;
	map<wstring, DirectX11Texture> mTextureBuffer;

	XMFLOAT4X4 mScale;
	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
	XMFLOAT4X4 mAnchor;
	XMFLOAT4X4 mRotation;

	ID3D11Buffer *mCBAnchor = NULL;
	ID3D11Buffer *mCBScale = NULL;
	ID3D11Buffer *mCBWorld = NULL;
	ID3D11Buffer *mCBView = NULL;
	ID3D11Buffer *mCBProjection = NULL;
	ID3D11Buffer *mCBRotation = NULL;
};
