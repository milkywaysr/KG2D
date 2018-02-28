#include "KG2d.h"
#include "KGScheduler.h"


KGScheduler::KGScheduler(function<void(float)> callBackFun, int fps, int times):
mCallBackFun(callBackFun),
mTotalTime(0.0f),
mFrameTime(0.0f),
mTimes(times)
{
	mFrameTime = 1.0f / static_cast<float>(fps);
}


KGScheduler::~KGScheduler()
{
}

bool KGScheduler::update(float deltaTime)
{
	if (mTimes != 0)
	{
		mTotalTime += deltaTime;
		if (mTotalTime - mFrameTime > 0.0)
		{
			mCallBackFun(mTotalTime);
			mTotalTime = 0.0f;

			if (mTimes != -1)
				mTimes--;
		}
	}
	return (mTimes != 0);
}

int KGScheduler::GetTag()
{
	return mTag;
}

void KGScheduler::SetTag(int tag)
{
	mTag = tag;
}
