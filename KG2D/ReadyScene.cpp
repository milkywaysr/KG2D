#include <math.h>
#include "ReadyScene.h"
#include "GameInfo.h"

//边栏文字颜色
const KGColorRGB nomal_color = { 195, 200, 204 };
const KGColorRGB select_color = {0, 127, 205};

const KGColorRGB tech_nomal = { 50, 100, 250 };
const KGColorRGB tech_select = {0, 0, 0};
const KGColorRGB tech_research = { 255, 67, 67 };
const KGColorRGB tech_invalid = { 119, 119, 119 };
const KGColorRGB tech_color[4] = {
	tech_nomal,
	tech_select,
	tech_research,
	tech_invalid };

const KGColorRGB info_color = { 0, 0, 0 };

const KGColorRGB object_color[3] = {
	tech_nomal,
	tech_select,
	tech_invalid
};

ReadyScene::ReadyScene()
{
	//背景
	KGSprite *bg = new KGSprite(wstring(L"resources/ready_bg.dds"));
	bg->SetPosition(-400, -300);
	bg->SetZOrder(1);
	this->AddShaderObject(bg);

	//边栏按钮 默认选科技
	mTech = new KGLabel(wstring(L"科技树"), 30, select_color);
	mTech->SetPosition(45, 430);
	this->AddShaderObject(mTech);

	mObject = new KGLabel(wstring(L"选择目标"), 30, nomal_color);
	mObject->SetPosition(30, 495);
	this->AddShaderObject(mObject);

	mExit = new KGLabel(wstring(L"退出"), 30, nomal_color);
	mExit->SetPosition(55, 560);
	this->AddShaderObject(mExit);

	KGLabel *techLabelA = new KGLabel(wstring(L"数据收集率"), 30, nomal_color);
	techLabelA->SetPosition(20, 150);
	this->AddShaderObject(techLabelA);

	KGLabel *techLabelR = new  KGLabel(wstring(L"发射距离"), 30, nomal_color);
	techLabelR->SetPosition(30, 290);
	this->AddShaderObject(techLabelR);

	//科技子画面
	mCurWnd = TechWnd;
	KGSprite *techBg = new KGSprite(wstring(L"resources/ready_tech_bg.dds"));
	techBg->SetPosition(-400, -300);
	this->AddShaderObject(techBg);
	mTechWndList.push_back(techBg);

	//目标子界面
	KGSprite *objectBg = new KGSprite(wstring(L"resources/ready_object_bg.dds"));
	objectBg->SetPosition(-400, -300);
	objectBg->SetVisable(false);
	this->AddShaderObject(objectBg);
	mObjectWndList.push_back(objectBg);

	KGLabel *surveyLevel = new KGLabel(wstring(L"探测等级"), 30, tech_nomal);
	surveyLevel->SetPosition(250, 500);
	surveyLevel->SetVisable(false);
	this->AddShaderObject(surveyLevel);
	mObjectWndList.push_back(surveyLevel);

	mStart = new KGLabel(wstring(L"开始探测"), 30, tech_nomal);
	mStart->SetPosition(550, 500);
	mStart->SetVisable(false);
	this->AddShaderObject(mStart);
	mObjectWndList.push_back(mStart);
}


ReadyScene::~ReadyScene()
{
	this->DeleteShanderObjectList();
}

void ReadyScene::Update(float deltaTime)
{
	//检测鼠标点击事件
	KGPoint pos = KG2D::Input->GetMousePosition();
	if (KG2D::Input->IsMouseLeftKeyUp())
	{
		//左边栏
		if (CheckLabelHit(pos, *mTech))
		{
			mCurWnd = TechWnd;
			mTech->SetFontColor(select_color);
			mObject->SetFontColor(nomal_color);

			for (auto tmp : mTechWndList)
				tmp->SetVisable(true);
			for (auto tmp : mObjectWndList)
				tmp->SetVisable(false);
		}
		else if (CheckLabelHit(pos, *mObject))
		{
			mCurWnd = ObjectWnd;
			mTech->SetFontColor(nomal_color);
			mObject->SetFontColor(select_color);

			for (auto tmp : mObjectWndList)
				tmp->SetVisable(true);
			for (auto tmp : mTechWndList)
				tmp->SetVisable(false);
		}
		else if (CheckLabelHit(pos, *mExit))
		{
			GameInfo::ShareGameInfo()->SwitchToStartScene();
		}
		//科技树
		if (mCurWnd == TechWnd)
		{
			for (int i = 0; i < 4; i++)
			{
				if (iGameInfo->GetRocketStateByLevel(i) == Tech_Nomal)
				{
					if (CheckLabelHit(pos, *mRocketLabel[i]))
					{
						iGameInfo->SetCurRocket(i);
						break;
					}
				}
				if (iGameInfo->GetRocketStateByLevel(i) == Tech_Research)
				{
					if (CheckLabelHit(pos, *mRocketLabel[i]))
					{
						iGameInfo->ResearchRocket(i);
						break;
					}
				}
				if (iGameInfo->GetApparatusStateByLevel(i) == Tech_Nomal)
				{
					if (CheckLabelHit(pos, *mApparatusLabel[i]))
					{
						iGameInfo->SetCurApparatus(i);
						break;
					}
				}
				if (iGameInfo->GetApparatusStateByLevel(i) == Tech_Research)
				{
					if (CheckLabelHit(pos, *mApparatusLabel[i]))
					{							
						iGameInfo->ResearchApparatus(i);
						break;
					}
				}
			}
		}
		
		//目标
		if (mCurWnd == ObjectWnd)
		{
			if (CheckLabelHit(pos, *mStart))
			{
				iGameInfo->SwitchToGameScene();
			}
			for (int i = 0; i < 7; i++)
			{
				if (iGameInfo->GetObjectStateById(i) == Object_Nomal)
				{
					if (CheckLabelHit(pos, *mObjectLabel[i]))
					{
						iGameInfo->SetCurObject(i);
						break;
					}
				}
			}
		}
	}
}

void ReadyScene::SetData()
{
	//如果放到构造函数中会导致死循环，所以放到这里设置
	//科技点
	int sp = iGameInfo->GetSciencePoint();
	mSciencePoint = new KGLabel(to_wstring(sp), 25, nomal_color);
	mSciencePoint->SetPosition(140 - mSciencePoint->GetLabelSize().width,
								50);
	this->AddShaderObject(mSciencePoint);

	int ad = iGameInfo->GetCurApparatus().data;
	mApparatusData = new KGLabel(to_wstring(ad), 25, nomal_color);
	mApparatusData->SetPosition(80, 200);
	this->AddShaderObject(mApparatusData);

	int rd = iGameInfo->GetCurRocket().data;
	mRocketData = new KGLabel(to_wstring(rd), 25, nomal_color);
	mRocketData->SetPosition(80, 340);
	this->AddShaderObject(mRocketData);

	//科技树
	for (int i = 0; i < 4; i++)
	{
		TechState state = iGameInfo->GetRocketStateByLevel(i);
		TechInfo info = iGameInfo->GetRocketInfoByLevel(i);
		mRocketLabel[i] = new KGLabel(info.name, 30, tech_color[state]);
		mRocketLabel[i]->SetPosition(298, 175 + i * 80);
		this->AddShaderObject(mRocketLabel[i]);
		mTechWndList.push_back(mRocketLabel[i]);

		state = iGameInfo->GetApparatusStateByLevel(i);
		info = iGameInfo->GetApparatusInfoByLevel(i);
		mApparatusLabel[i] = new KGLabel(info.name, 30, tech_color[state]);
		mApparatusLabel[i]->SetPosition(580, 175 + i * 80);
		this->AddShaderObject(mApparatusLabel[i]);
		mTechWndList.push_back(mApparatusLabel[i]);
	}

	//选择目标
	for (int i = 0; i < 7; i++)
	{
		ObjectState state = iGameInfo->GetObjectStateById(i);
		ObjectInfo info = iGameInfo->GetObjectInfoById(i);

		mObjectLabel[i] = new KGLabel(info.name, 30, object_color[state]);
		mObjectLabel[i]->SetPosition(250, 120 + i * 50);
		mObjectLabel[i]->SetVisable(false);

		this->AddShaderObject(mObjectLabel[i]);
		mObjectWndList.push_back(mObjectLabel[i]);

		mObjectDstLabel[i] = new KGLabel(to_wstring(abs(info.distance)), 
			30, object_color[state]);
		mObjectDstLabel[i]->SetPosition(370, 120 + i * 50);
		mObjectDstLabel[i]->SetVisable(false);
		
		this->AddShaderObject(mObjectDstLabel[i]);
		mObjectWndList.push_back(mObjectDstLabel[i]);
	}
	//简介
	mObjectInfo = new KGLabel(iGameInfo->GetCurObjectIntroduction(),
		21, info_color);
	mObjectInfo->SetPosition(460, 160);
	mObjectInfo->SetLabelSize(KGSize(300, 300));
	mObjectInfo->SetVisable(false);

	this->AddShaderObject(mObjectInfo);
	mObjectWndList.push_back(mObjectInfo);

	int sv = iGameInfo->GetCurObjectSurveyLevel();
	mObjectSurveyLevel = new KGLabel(to_wstring(sv), 30, tech_nomal);
	mObjectSurveyLevel->SetPosition(380, 500);
	mObjectSurveyLevel->SetVisable(false);

	this->AddShaderObject(mObjectSurveyLevel);
	mObjectWndList.push_back(mObjectSurveyLevel);
}

void ReadyScene::UpdateData()
{
	int sp = iGameInfo->GetSciencePoint();
	mSciencePoint->SetString(to_wstring(sp));
	mSciencePoint->SetPosition(140 - mSciencePoint->GetLabelSize().width,
		50);

	int ad = iGameInfo->GetCurApparatus().data;
	mApparatusData->SetString(to_wstring(ad));

	int rd = iGameInfo->GetCurRocket().data;
	mRocketData->SetString(to_wstring(rd));

	for (int i = 0; i < 4; i++)
	{
		TechState state = iGameInfo->GetRocketStateByLevel(i);
		mRocketLabel[i]->SetFontColor(tech_color[state]);

		state = iGameInfo->GetApparatusStateByLevel(i);
		mApparatusLabel[i]->SetFontColor(tech_color[state]);
	}

	for (int i = 0; i < 7; i++)
	{
		ObjectState state = iGameInfo->GetObjectStateById(i);
		mObjectLabel[i]->SetFontColor(object_color[state]);
		mObjectDstLabel[i]->SetFontColor(object_color[state]);
	}
	mObjectInfo->SetString(iGameInfo->GetCurObjectIntroduction());

	int sv = iGameInfo->GetCurObjectSurveyLevel();
	mObjectSurveyLevel->SetString(to_wstring(sv));
}
