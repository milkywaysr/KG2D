#pragma once
class KGPoint
{
public:
	KGPoint();
	KGPoint(float x, float y);
	~KGPoint();

	void SetPosition(float _x, float _y);
	KGPoint operator+(KGPoint p2);
	KGPoint operator-(KGPoint p2);
	KGPoint operator=(KGPoint p2);
	bool operator==(KGPoint p2);
public:
	float x = 0.0f;
	float y = 0.0f;
};

extern KGPoint ZeroPoint;