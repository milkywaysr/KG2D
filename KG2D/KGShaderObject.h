#pragma once
#include "KGPoint.h"
class KGShaderObject
{
public:
	KGShaderObject();
	virtual ~KGShaderObject();
	void SetPosition(float x, float y);
	void SetPosition(KGPoint p);
	const KGPoint& GetPosition() const;
	void SetVisable(bool visible);
	bool IsVisible();

protected:
	KGPoint mPosition;	//λ��
	bool mIsVisible = true;	//�Ƿ�ɼ�
};

