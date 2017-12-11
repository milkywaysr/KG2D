#pragma once
#include <string>
#include <vector>
#include "KG2D.h"
using namespace std;

enum ChildType {TechWnd, ObjectWnd}; //�����Ӵ���
class ReadyScene : public KGScene
{
public:
	ReadyScene();
	~ReadyScene();
	void Update(float deltaTime);

	void SetData();
	void UpdateData();

private:
	KGLabel *mTech;		//�Ƽ�����ť
	KGLabel *mObject;	//ѡ��Ŀ�갴ť
	KGLabel *mExit;		//�˳���ť
	KGLabel *mSciencePoint;
	KGLabel *mRocketData;
	KGLabel *mApparatusData;

	//�Ƽ��ӻ���
	vector<KGShaderObject*> mTechWndList;
	KGLabel *mRocketLabel[4];
	KGLabel *mApparatusLabel[4];

	//Ŀ���ӻ���
	vector<KGShaderObject*> mObjectWndList;
	KGLabel *mObjectLabel[7];
	KGLabel *mObjectDstLabel[7];
	KGLabel *mObjectInfo;
	KGLabel *mObjectSurveyLevel;
	KGLabel *mStart;

	ChildType mCurWnd;
};

