#include "KGPoint.h"

KGPoint ZeroPoint(0.0f, 0.0f);

KGPoint::KGPoint()
{
}

KGPoint::KGPoint(float _x, float _y):x(_x), y(_y)
{
}


KGPoint::~KGPoint()
{
}

void KGPoint::SetPosition(float _x, float _y)
{
	x = _x;
	y = _y;
}

KGPoint KGPoint::operator+(KGPoint p2)
{
	return KGPoint(x+p2.x, y+p2.y);
}

KGPoint KGPoint::operator-(KGPoint p2)
{
	return KGPoint(x-p2.x, y-p2.y);
}

KGPoint KGPoint::operator=(KGPoint p2)
{
	x = p2.x;
	y = p2.y;
	return KGPoint(x, y);
}

bool KGPoint::operator==(KGPoint p2)
{
	return (x == p2.x && y == p2.y);
}
