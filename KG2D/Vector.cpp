#include "Vector.h"


Vector::Vector(void)
{
}


Vector::~Vector(void)
{
}


Vector::Vector(float x, float y)
{
	vectorX = x;
	vectorY = y;
}


float Vector::Module()
{
	return sqrt(pow(vectorX, 2.0f) + pow(vectorY, 2.0f));
}



Vector operator+(Vector v1, Vector v2)
{
	return Vector(v1.vectorX+v2.vectorX, v1.vectorY + v2.vectorY);
}

Vector operator-(Vector v1, Vector v2)
{
	return Vector(v1.vectorX - v2.vectorX, v1.vectorY - v2.vectorY);
}

Vector operator *(Vector x, float y)
{
	return Vector(x.vectorX * y, x.vectorY * y);
}

Vector operator *(float y, Vector x)
{
	return Vector(x.vectorX * y, x.vectorY * y);
}

Vector operator /(Vector x, float y)
{
	return Vector(x.vectorX / y, x.vectorY / y);
}

Vector operator /(float y, Vector x)
{
	return Vector(x.vectorX / y, x.vectorY / y);
}
