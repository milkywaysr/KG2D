#include "KGTimer.h"


KGTimer::KGTimer():
mBaseTime(0),
mCurrTime(0),
mPrevTime(0),
mSecondsPerCount(0.0),
mDeltaTime(-1.0),
mStopped(false),
mTagCount(0)
{
	//完成Timer的初始化工作
	_int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}


KGTimer::~KGTimer()
{
	//Timer资源释放的工作
}

float KGTimer::DeltaTime() const
{
	return static_cast<float>(mDeltaTime);
}

void KGTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
}

void KGTimer::Tick()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;
	mPrevTime = mCurrTime;

	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}

	for (auto &scheduler : mSchedulerList)
	{
		scheduler.update(mDeltaTime);
	}
}

int KGTimer::RegisterScheduler(KGScheduler scheduler)
{
	scheduler.SetTag(mTagCount);
	mSchedulerList.push_back(scheduler);
	return mTagCount++;
}

void KGTimer::UnRegisterScheduler(int tag)
{
	auto i = mSchedulerList.begin();
	while (i != mSchedulerList.end())
	{
		if (i->GetTag() == tag)
		{
			mSchedulerList.erase(i);
			break;
		}
		else
		{
			i++;
		}
	}
}
