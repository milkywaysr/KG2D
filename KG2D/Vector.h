#pragma once
#include <math.h>

class Vector
{
public:
	Vector(void);
	Vector(float x, float y);
	~Vector(void);
	
	float Module();
	
	float vectorX = 0.0f;
	float vectorY = 0.0f;
};

Vector operator +(Vector v1, Vector v2);
Vector operator -(Vector v1, Vector v2);
Vector operator *(Vector x, float y);
Vector operator *(float y, Vector x);
Vector operator /(Vector x, float y);
Vector operator /(float y, Vector x);