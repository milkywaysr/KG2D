#include "KGSprite.h"
#include <DirectXMath.h>
#include "KG2d.h"


KGSprite::KGSprite()
{
}

KGSprite::KGSprite(wstring & fileName)
{
	mTexture = KG2D::Shader->CreateTextureWithFile(fileName);
	mSize = mTexture.getSize();
}

KGSprite::~KGSprite()
{
}

const KGPoint & KGSprite::GetAnchor() const
{
	return mAnchor;
}

const KGSize & KGSprite::GetSize() const
{
	return mSize;
}

const KGTexture & KGSprite::GetTexture() const
{
	return mTexture;
}

float KGSprite::GetZOrder() const
{
	return mZOrder;
}

float KGSprite::GetRotation() const
{
	return mRotation;
}

void KGSprite::SetAnchor(KGPoint & anchor)
{
	mAnchor = anchor;
}

void KGSprite::SetRotation(float rotation)
{
	mRotation = DirectX::XMConvertToRadians(rotation);
}

void KGSprite::SetSize(KGSize & size)
{
	mSize = size;
}

void KGSprite::SetZOrder(float z_order)
{
	mZOrder = z_order;
}
