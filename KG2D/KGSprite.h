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
	KGPoint mAnchor;	//�����ê��(0, 0)���½ǡ�(1, 1)���Ͻ�
	KGSize mSize;		//����Ĵ�С
	KGTexture mTexture;	//����
	float mRotation = 0.0f;
	float mZOrder = 0.0f;		//�������
};

