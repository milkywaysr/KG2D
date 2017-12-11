#include "KGShaderFactory.h"


KGShaderFactory::KGShaderFactory()
{
}


KGShaderFactory::~KGShaderFactory()
{
}

std::shared_ptr<KGShaderInterface> KGShaderFactory::GetShaderInterface(HWND hwnd, ShaderType st)
{
	switch (st)
	{
	case DirectX11Shader:
		return std::shared_ptr<KGShaderInterface>(new KGDirectX11Shader(hwnd));
		break;
	default:
		break;
	}
	return nullptr;
}
