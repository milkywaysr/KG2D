#pragma once
#include <memory>
#include "KGShaderInterface.h"
#include "KGDirectX11Shader.h"

//支持的渲染类型
enum ShaderType
{
	DirectX11Shader = 1
};

class KGShaderFactory
{
public:
	KGShaderFactory();
	~KGShaderFactory();

	static std::shared_ptr<KGShaderInterface> GetShaderInterface(HWND hwnd, ShaderType st);
};

