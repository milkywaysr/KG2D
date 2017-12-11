#pragma once
#include "KGShaderObject.h"
#include "KGSize.h"
#include "KGTexture.h"


class KGSprite :
	public KGShaderObject
{
public:
	KGSprite();
	KGSprite(wstring &fileName);
	~KGSprite();

public:
	const KGPoint& GetAnchor() const;
	const KGSize& GetSize() const;
	const KGTexture& GetTexture() const;
	float GetZOrder() const;
	float GetRotation() const;

	void SetAnchor(KGPoint& anchor);
	void SetRotation(float rotation);
	void SetSize(KGSize& size);
	void SetZOrder(float z_order);
protected:
	KGPoint mAnchor;	//精灵的锚点(0, 0)左下角　(1, 1)右上角
	KGSize mSize;		//精灵的大小
	KGTexture mTexture;	//纹理
	float mRotation = 0.0f;
	float mZOrder = 0.0f;		//精灵深度
};

