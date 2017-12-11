#include "KGCamera.h"


KGCamera::KGCamera()
{
}


KGCamera::~KGCamera()
{
}

void KGCamera::SetCenterPoint(const KGPoint & point)
{
	mCenterPoint = point;
}

const KGPoint & KGCamera::GetCenterPoint() const
{
	return mCenterPoint;
}
