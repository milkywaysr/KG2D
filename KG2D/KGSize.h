#pragma once
class KGSize
{
public:
	KGSize();
	KGSize(float w, float h);
	~KGSize();

public:
	float width = 0.0f;
	float higth = 0.0f;
};

extern KGSize ZeroSize;