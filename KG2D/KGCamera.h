#pragma once
#include "KGPoint.h"
/*因为没有需求所以暂时不添加相机缩放功能*/

class KGCamera
{
public:
	KGCamera();
	~KGCamera();

	void SetCenterPoint(const KGPoint&);
	const KGPoint& GetCenterPoint() const;
private:
	KGPoint mCenterPoint;   //相机的中心点
};

