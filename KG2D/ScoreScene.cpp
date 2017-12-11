#include "ScoreScene.h"
#include "GameInfo.h"

const KGColorRGB result_color = { 200, 200 ,255 };
const KGColorRGB button_color = { 220, 220, 220 };
ScoreScene::ScoreScene()
{
	KGSprite *bg = new KGSprite(wstring(L"resources/score_bg.dds"));
	bg->SetPosition(-400, -300);
	this->AddShaderObject(bg);

	mReturn = new KGLabel(wstring(L"返回准备界面"), 30, button_color);
	mReturn->SetPosition(320, 400);
	this->AddShaderObject(mReturn);
}


ScoreScene::~ScoreScene()
{
}

void ScoreScene::Update(float deltaTime)
{
	if (KG2D::Input->IsMouseLeftKeyUp())
	{
		KGPoint pos = KG2D::Input->GetMousePosition();
		if (CheckLabelHit(pos, *mReturn))
		{
			iGameInfo->ProcessSurveyResult(mSurveyPoint, mMoney);
			iGameInfo->SwitchToReadyScene();
		}
	}
}

void ScoreScene::Init()
{
	mResult = new KGLabel(wstring(L""), 25, result_color);
	mResult->SetPosition(220, 275);
	this->AddShaderObject(mResult);
}

void ScoreScene::SetData(int surveyPoint)
{
	mSurveyPoint = surveyPoint;
	mMoney = surveyPoint * iGameInfo->GetCurObject().surveyMoney;
	wstring name = iGameInfo->GetCurObject().name;
	wstring ret = L"获得" + name + L"科研点数" + to_wstring(mSurveyPoint)
		+ L",科研经费" + to_wstring(mMoney);
	mResult->SetString(ret);
}
