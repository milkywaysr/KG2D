#include "KGLabel.h"

KGLabel::KGLabel(wstring label, float fontSize, KGColorRGB fontColor, KGPoint position):
	mLabel(label), 
	mFontColor(fontColor), 
	mFontSize(fontSize),
	mLimitSize(ZeroSize)
{
	SetPosition(position);
	mSize = KG2D::Shader->CalculateLabelSize(mLabel, mFontSize, mLimitSize);
	//´´½¨ÎÆÀí

}

const KGColorRGB & KGLabel::GetColor() const
{
	return mFontColor;
}

const float KGLabel::GetFontSize() const
{
	return mFontSize;
}

const KGSize & KGLabel::GetLabelSize() const
{
	return mSize;
}

const wstring & KGLabel::GetLabel() const
{
	return mLabel;
}

void KGLabel::SetString(wstring & str)
{
	mLabel = str;
	mSize = KG2D::Shader->CalculateLabelSize(mLabel, mFontSize, mLimitSize);
}

void KGLabel::SetLabelSize(KGSize size)
{
	mLimitSize = size;
	mSize = KG2D::Shader->CalculateLabelSize(mLabel, mFontSize, mLimitSize);
}

void KGLabel::SetFontColor(KGColorRGB fc)
{
	mFontColor = fc;
}

KGLabel::~KGLabel()
{
}
