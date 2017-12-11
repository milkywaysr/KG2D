#include "KG2DTool.h"
#include "KGLabel.h"

bool CheckLabelHit(const KGPoint& pos, const KGLabel& label)
{
	bool ret = false;
	KGPoint lPos = label.GetPosition();
	KGSize lSize = label.GetLabelSize();
	if (pos.x > lPos.x &&
		pos.x < lPos.x + lSize.width)

	{
		if (pos.y > lPos.y &&
			pos.y < lPos.y + lSize.higth)
		{
			ret = true;
		}
	}
	return ret;
}