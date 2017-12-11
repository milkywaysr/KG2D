#pragma once
#include <string>
#include "KGShaderObject.h"
#include "KGSize.h"
#include "KG2DTool.h"
#include "KG2d.h"
using namespace std;

class KGLabel :
	public KGShaderObject
{
public:
	KGLabel(wstring label, float fontSize, 
		KGColorRGB fontColor = {0, 0, 0}, //Ä¬ÈÏºÚÉ« 
		KGPoint position = ZeroPoint
);
	const KGColorRGB& GetColor() const;
	const float GetFontSize() const;
	const KGSize& GetLabelSize() const;
	const wstring& GetLabel() const;

	void SetString(wstring &str);
	void SetLabelSize(KGSize size);
	void SetFontColor(KGColorRGB fc);
	~KGLabel();

protected:
	KGSize mSize;
	KGSize mLimitSize;
	KGColorRGB mFontColor;
	KGTexture mTexture;
	wstring mLabel;
	float mFontSize;
};

