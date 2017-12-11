#pragma once
#include <string>
#include <list>
#include "KGSize.h"
#include "KGShaderObject.h"
#include "KGCamera.h"
#include "KGTexture.h"
using namespace std;

extern int g_fps;
class KGShaderInterface
{
public:
	KGShaderInterface();
	virtual ~KGShaderInterface();
public:
	//渲染引擎支持的操作放这里
	//渲染画面
	virtual void Render(const list<KGShaderObject*>&, const KGCamera&) = 0;
	//创建一个纹理
	virtual KGTexture CreateTextureWithFile(wstring &fileName) = 0;
	//计算文本大小
	virtual KGSize CalculateLabelSize(wstring &label, float fontSize, KGSize) = 0;
};

	