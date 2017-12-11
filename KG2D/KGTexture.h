#pragma once
#include <string>
#include "KGSize.h"
using namespace std;

class KGTexture
{
public:
	KGTexture();
	~KGTexture();
	void SetTextureData(wstring fileName, KGSize size);
	KGSize getSize() const;
	const wstring& GetTextureName() const;
protected:
	wstring mTextureFileName = L"";
	KGSize mTextureSize;
};

