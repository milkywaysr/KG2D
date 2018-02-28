#include "KGShaderObject.h"


KGShaderObject::KGShaderObject()
{
}


KGShaderObject::~KGShaderObject()
{
}

void KGShaderObject::SetPosition(float x, float y)
{
	mPosition.SetPosition(x, y);
}

void KGShaderObject::SetPosition(KGPoint p)
{
	mPosition = p;
}

const KGPoint & KGShaderObject::GetPosition() const
{
	return mPosition;
}

void KGShaderObject::SetVisable(bool visible)
{
	mIsVisible = visible;
}

bool KGShaderObject::IsVisible()
{
	return mIsVisible;
}
