#pragma once
#include <functional>
using namespace std;

#define MemberFunScheduler(_TARGET_, _FPS_, _TIMES_)\
KGScheduler(([this](float deltaTime) {this->_TARGET_(deltaTime); }), _FPS_, _TIMES_)

class KGScheduler
{
public:
	KGScheduler(function<void(float)> callBackFun, int fps, int times);
	~KGScheduler();

public:
	bool update(float deltaTime);
	int GetTag();
	void SetTag(int tag);
private:
	function<void(float)> mCallBackFun;
	float mTotalTime;	//������ʱ��
	float mFrameTime;	//һ֡��ʱ��
	int	mTimes;			//����
	int mTag;			//��ʾ��ע��������ʱ����
};