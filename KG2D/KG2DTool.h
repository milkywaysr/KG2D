#pragma once

#include <Windows.h>
#include "KGPoint.h"


#define HR(_HR_, _TEXT_) \
if (FAILED(_HR_)) \
	MessageBox(NULL, L##_TEXT_, L"Error", NULL)

#define KG_LOG(MSG) std::cout << (MSG)

struct KGColorRGB
{
	float red;
	float green;
	float blue;
};

class KGLabel;
bool CheckLabelHit(const KGPoint&, const KGLabel&);