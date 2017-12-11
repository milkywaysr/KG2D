#pragma once
#include <string>
#include <vector>
#include "KG2D.h"
using namespace std;

enum ChildType {TechWnd, ObjectWnd}; //两个子窗口
class ReadyScene : public KGScene
{
public:
	ReadyScene();
	~ReadyScene();
	void Update(float deltaTime);

	void SetData();
	void UpdateData();

private:
	KGLabel *mTech;		//科技树按钮
	KGLabel *mObject;	//选择目标按钮
	KGLabel *mExit;		//退出按钮
	KGLabel *mSciencePoint;
	KGLabel *mRocketData;
	KGLabel *mApparatusData;

	//科技子画面
	vector<KGShaderObject*> mTechWndList;
	KGLabel *mRocketLabel[4];
	KGLabel *mApparatusLabel[4];

	//目标子画面
	vector<KGShaderObject*> mObjectWndList;
	KGLabel *mObjectLabel[7];
	KGLabel *mObjectDstLabel[7];
	KGLabel *mObjectInfo;
	KGLabel *mObjectSurveyLevel;
	KGLabel *mStart;

	ChildType mCurWnd;
};

