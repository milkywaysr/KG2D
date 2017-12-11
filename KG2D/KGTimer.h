#pragma once
#include <memory>
#include <vector>
#include <windows.h>
#include "KGScheduler.h"

class KGTimer
{
public:
	KGTimer();
	~KGTimer();

public:
	//float GameTime() const;
	float DeltaTime() const;
	void Reset();				//在进入主循环之前调用
	//void Start();				//暂停的时候开始计时
	//void Stop();				//暂停
	void Tick();				//在主循环中调用
	
	int RegisterScheduler(KGScheduler scheduler);
	void UnRegisterScheduler(int tag);
private:
	__int64 mBaseTime;			//
	__int64 mCurrTime;			//当前时间
	__int64 mPrevTime;			//上一帧时间

	double mSecondsPerCount;	//一秒的时钟数
	double mDeltaTime;			//时间间隔

	bool mStopped;

	int mTagCount;
	vector<KGScheduler> mSchedulerList;
};