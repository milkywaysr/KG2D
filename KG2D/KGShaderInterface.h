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
	//��Ⱦ����֧�ֵĲ���������
	//��Ⱦ����
	virtual void Render(const list<KGShaderObject*>&, const KGCamera&) = 0;
	//����һ������
	virtual KGTexture CreateTextureWithFile(wstring &fileName) = 0;
	//�����ı���С
	virtual KGSize CalculateLabelSize(wstring &label, float fontSize, KGSize) = 0;
};

	