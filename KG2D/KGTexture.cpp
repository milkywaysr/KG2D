#include "KGTexture.h"


KGTexture::KGTexture()
{
}


KGTexture::~KGTexture()
{
}

void KGTexture::SetTextureData(wstring fileName, KGSize size)
{
	mTextureFileName = fileName;
	mTextureSize = size;
}

const wstring & KGTexture::GetTextureName() const
{
	return mTextureFileName;
}

KGSize KGTexture::getSize() const
{
	return mTextureSize;
}
