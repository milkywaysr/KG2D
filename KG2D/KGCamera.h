#pragma once
#include "KGPoint.h"
/*��Ϊû������������ʱ�����������Ź���*/

class KGCamera
{
public:
	KGCamera();
	~KGCamera();

	void SetCenterPoint(const KGPoint&);
	const KGPoint& GetCenterPoint() const;
private:
	KGPoint mCenterPoint;   //��������ĵ�
};

