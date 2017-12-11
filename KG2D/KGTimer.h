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
	void Reset();				//�ڽ�����ѭ��֮ǰ����
	//void Start();				//��ͣ��ʱ��ʼ��ʱ
	//void Stop();				//��ͣ
	void Tick();				//����ѭ���е���
	
	int RegisterScheduler(KGScheduler scheduler);
	void UnRegisterScheduler(int tag);
private:
	__int64 mBaseTime;			//
	__int64 mCurrTime;			//��ǰʱ��
	__int64 mPrevTime;			//��һ֡ʱ��

	double mSecondsPerCount;	//һ���ʱ����
	double mDeltaTime;			//ʱ����

	bool mStopped;

	int mTagCount;
	vector<KGScheduler> mSchedulerList;
};