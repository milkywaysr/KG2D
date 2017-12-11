#include <random>
#include <sstream>
#include "GameScene.h"

using namespace std;

#define PI 3.1415f
#define G 8.0f
#define MAX_SPEED 60.0f

KGColorRGB label_color = {200, 200, 220};

GameScene::GameScene()
{
	mCurState = Game_View;
	mCameraLimit = 6;

	mAimSpeed = 0.01f;
	mAimAngle = 0.0f;

	KGLabel *spLabel = new KGLabel(wstring(L"已获得探测点"), 25, label_color);
	spLabel->SetPosition(550, 0);
	this->AddShaderObject(spLabel);

	mSurveyPoint = 0;
	mTrueSurveyPoint = 0.0f;
	mSurveyPointLabel = new KGLabel(wstring(L"0"), 25, label_color);
	mSurveyPointLabel->SetPosition(700, 0);
	this->AddShaderObject(mSurveyPointLabel);

	KGSprite *bg = new KGSprite(wstring(L"resources/game_bg.dds"));
	bg->SetPosition(-400, -300);
	bg->SetZOrder(1);
	this->AddShaderObject(bg);

	mAim = new KGSprite(wstring(L"resources/aim.dds"));
	mAim->SetAnchor(KGPoint(0.5f, -1.2f));
	mAim->SetZOrder(0.5f);
	this->AddShaderObject(mAim);
	mAimList.push_back(mAim);

	KGLabel *aimSpeed = new KGLabel(wstring(L"发射速度"), 25, label_color);
	aimSpeed->SetPosition(500, 280);
	this->AddShaderObject(aimSpeed);
	mAimList.push_back(aimSpeed);

	KGLabel *aimAngle = new KGLabel(wstring(L"发射角度"), 25, label_color);
	aimAngle->SetPosition(500, 310);
	this->AddShaderObject(aimAngle);
	mAimList.push_back(aimAngle);

	mSpeedLabel = new KGLabel(wstring(L"0.01"), 25, label_color);
	mSpeedLabel->SetPosition(600, 280);
	this->AddShaderObject(mSpeedLabel);
	mAimList.push_back(mSpeedLabel);

	mAngleLabel = new KGLabel(wstring(L"0.00"), 25, label_color);
	mAngleLabel->SetPosition(600, 310);
	this->AddShaderObject(mAngleLabel);
	mAimList.push_back(mAngleLabel);

	mObjectArrow = new KGSprite(wstring(L"resources/arrow.dds"));
	mObjectArrow->SetAnchor(KGPoint(0.5f, 1.0f));
	this->AddShaderObject(mObjectArrow);

	mCircle = new KGSprite(wstring(L"resources/circle.dds"));
	mCircle->SetAnchor(KGPoint(0.5f, 0.5f));
	this->AddShaderObject(mCircle);

	mSatellite = new KGSprite(wstring(L"resources/satellite.dds"));
	mSatellite->SetAnchor(KGPoint(0.5f, 0.5f));
	mSatellite->SetPosition(200, 0);
	this->AddShaderObject(mSatellite);
	mRuningList.push_back(mSatellite);

	//帮助
	mHelpView = new KGLabel(wstring(L"使用上下方向键来观察各个天体位置，按Enter进入瞄准"), 20, label_color);
	mHelpView->SetLabelSize(KGSize(150, 300));
	mHelpView->SetPosition(10, 200);
	this->AddShaderObject(mHelpView);

	KGLabel *aimHelp =
		new KGLabel(wstring(L"使用方向键控制发射速度和发射角度，按A、D可微调发射角度，按空格发射"), 20, label_color);
	aimHelp->SetLabelSize(KGSize(150, 300));
	aimHelp->SetPosition(10, 200);
	this->AddShaderObject(aimHelp);
	mAimList.push_back(aimHelp);
}


GameScene::~GameScene()
{
	this->DeleteShanderObjectList();
}

void GameScene::Update(float deltaTime)
{
	if (KG2D::Input->IsKeyUp(DIK_ESCAPE))
	{
		iGameInfo->SwitchToReadyScene();
		return;
	}
	switch (mCurState)
	{
	case Game_View:
		OnView();
		break;
	case Game_Aim:
		OnAim();
		break;
	case Game_Runing:
		OnRuning(deltaTime);
		break;
	}
}

void GameScene::Init()
{
	//放入各个星球测试
	mObject[0] = new KGSprite(wstring(L"resources/mercury.dds"));
	mObject[1] = new KGSprite(wstring(L"resources/venus.dds"));
	mObject[2] = new KGSprite(wstring(L"resources/mars.dds"));
	mObject[3] = new KGSprite(wstring(L"resources/jupiter.dds"));
	mObject[4] = new KGSprite(wstring(L"resources/saturn.dds"));
	mObject[5] = new KGSprite(wstring(L"resources/uranus.dds"));
	mObject[6] = new KGSprite(wstring(L"resources/neptune.dds"));
	mObject[7] = new KGSprite(wstring(L"resources/earth.dds"));
	mObject[7]->SetAnchor(KGPoint(0.5f, 0.5f));
	this->AddShaderObject(mObject[7]);

	for (int i = 0; i < 7; i++)
	{
		mObject[i]->SetAnchor(KGPoint(0.5f, 0.5f));
		int dst = iGameInfo->GetObjectInfoById(i).distance;
		mObject[i]->SetPosition(0, 600 * abs(dst));
		this->AddShaderObject(mObject[i]);
	}
	mObject[1]->SetPosition(-200, 600);
	mObject[2]->SetPosition(200, 600);
}

void GameScene::CreateMission()
{
	mCurState = Game_View;
	mCamera.SetCenterPoint(ZeroPoint);

	mHelpView->SetVisable(true);
	SetAimShow(false);
	SetRuningShow(false);

	for (int i = 0; i < 7; i++)
	{	
		mObject[i]->SetVisable(false);
	}

	mSurveyPoint = 0;
	mTrueSurveyPoint = 0.0f;
	mSurveyPointLabel->SetString(to_wstring(mSurveyPoint));

	mMissionObjectList.clear();
	mTargetInfo = iGameInfo->GetCurObject();
	mMissionObjectList.push_back(mTargetInfo);
	mCameraLimit = abs(mTargetInfo.distance);

	static default_random_engine e;
	static uniform_int_distribution<unsigned> u(0, 1);

	//金星水星
	if (mTargetInfo.distance < 0)
	{
		for (int i = mTargetInfo.id + 1; i < 7 ; i++)
		{
			auto info = iGameInfo->GetObjectInfoById(i);
			if (info.distance < 0)
			{
				if (u(e))
				{
					mMissionObjectList.push_back(info);
				}
			}
			else
				break;
		}

	}
	//地球外侧
	else
	{
		int flag = mTargetInfo.id == 3 || mTargetInfo.id == 4;
		for (int i = mTargetInfo.id - 1; i > 0; i--) 
		{
			if ((i == 3 || i == 4) && flag)
				continue;
			auto info = iGameInfo->GetObjectInfoById(i);
			if (info.distance > 0)
			{
				
				if (u(e))
				{
					if (i == 3 || i == 4)
						flag = true;
					mMissionObjectList.push_back(info);
				}
			}
			else
				break;
		}
	}

	//设置各个天体的位置
	for (auto tmp : mMissionObjectList)
	{
		float range = 400 - tmp.surveyRadius;
		uniform_real<float> up(-range, range);
		float posx = up(e);
		mObject[tmp.id]->SetPosition(posx, abs(tmp.distance * 600));
		mObject[tmp.id]->SetVisable(true);
	}
	mCircle->SetPosition(mObject[mTargetInfo.id]->GetPosition());
	float circleSize = mTargetInfo.surveyRadius * 2;
	mCircle->SetSize(KGSize(circleSize, circleSize));

	mObjectArrow->SetPosition(CalObjectArrowPos());
}

void GameScene::OnView()
{
	KGPoint cpos = mCamera.GetCenterPoint();
	bool isMove = false;
	if (KG2D::Input->IsKeyDown(DIK_UP))
	{
		cpos.y += 5;
		if (cpos.y > 600 * mCameraLimit)
		cpos.y = 600 * mCameraLimit;
		mCamera.SetCenterPoint(cpos);
		isMove = true;
	}
	if (KG2D::Input->IsKeyDown(DIK_DOWN))
	{
		cpos.y -= 5;
		if (cpos.y < 0)
		cpos.y = 0;
		mCamera.SetCenterPoint(cpos);
		isMove = true;
	}
	if (isMove)
	{
		mObjectArrow->SetPosition(CalObjectArrowPos());
	}
	if (KG2D::Input->IsKeyUp(DIK_RETURN))
	{
		mCurState = Game_Aim;
		mCamera.SetCenterPoint(ZeroPoint);
		mHelpView->SetVisable(false);
		SetAimShow(true);
		mObjectArrow->SetPosition(CalObjectArrowPos());
	}
}

void GameScene::OnAim()
{
	if (KG2D::Input->IsKeyUp(DIK_SPACE))
	{
		SetAimShow(false);
		SetRuningShow(true);
		BeforeRuning();
		mCurState = Game_Runing;
	}
	if (KG2D::Input->IsKeyDown(DIK_UP))
	{
		mAimSpeed += 0.01f;
		if (mAimSpeed > 1.0f)
			mAimSpeed = 1.0f;

		KGSize sz = mAim->GetSize();
		sz.higth = 48 + 24 * mAimSpeed;
		mAim->SetSize(sz);
	}
	if (KG2D::Input->IsKeyDown(DIK_DOWN))
	{
		mAimSpeed -= 0.01f;
		if (mAimSpeed < 0.01f)
			mAimSpeed = 0.01f;

		KGSize sz = mAim->GetSize();
		sz.higth = 48 + 24 * mAimSpeed;
		mAim->SetSize(sz);
	}
	if (KG2D::Input->IsKeyDown(DIK_LEFT))
	{
		mAimAngle += 0.5;
		if (mAimAngle > 360.0f)
			mAimAngle -= 360.0f;
		mAim->SetRotation(mAimAngle);
	}
	if (KG2D::Input->IsKeyDown(DIK_RIGHT))
	{
		mAimAngle -= 0.5;
		if (mAimAngle < 0.0f)
			mAimAngle += 360.0f;
		mAim->SetRotation(mAimAngle);
	}
	if (KG2D::Input->IsKeyDown(DIK_A))
	{
		mAimAngle += 0.02;
		if (mAimAngle > 360.0f)
			mAimAngle -= 360.0f;
		mAim->SetRotation(mAimAngle);
	}
	if (KG2D::Input->IsKeyDown(DIK_D))
	{
		mAimAngle -= 0.02;
		if (mAimAngle < 0.0f)
			mAimAngle += 360.0f;
		mAim->SetRotation(mAimAngle);
	}
	UpdateAimData();
}

void GameScene::OnRuning(float deltaTime)
{
	UpdateSatellite(deltaTime);
	CheckGameOver();
	CheckSurveyPoint();
	mObjectArrow->SetPosition(CalObjectArrowPos());
	
}

void GameScene::BeforeRuning()
{
	//计算卫星位置
	KGPoint spos;
	float angle = mAimAngle * PI / 180.0f;

	spos.x = -(sin(angle) * 110);
	spos.y = cos(angle) * 110;
	mSatellite->SetPosition(spos);

	mSatelliteVelocity = Vector(-sin(angle), cos(angle)) * MAX_SPEED * mAimSpeed;
}

void GameScene::UpdateSatellite(float deltaTime)
{
	KGPoint spos = mSatellite->GetPosition();

	//计算地球提供的加速度
	Vector R(-spos.x, -spos.y);
	Vector A;
	float r;
	r = R.Module();
	A = ((G * 4900) / pow(r, 2)) * (R / r);
	
	//计算其他天体提供的加速度
	for (auto tmp : mMissionObjectList)
	{
		KGPoint opos = mObject[tmp.id]->GetPosition();
		KGPoint R2 = opos - spos;
		R = Vector(R2.x, R2.y);
		r = R.Module();
		Vector A2;
		float m = tmp.mass;
		A2 = ((G * m) / pow(r, 2)) * (R / r);
		A = A + A2;
	}
	Vector S = deltaTime * (mSatelliteVelocity + ((A / 2) * deltaTime));
	mSatelliteVelocity = mSatelliteVelocity + (A * deltaTime);
	spos.x += S.vectorX;
	spos.y += S.vectorY;
	mSatellite->SetPosition(spos);
	mCamera.SetCenterPoint(KGPoint(0, spos.y));
}

void GameScene::CheckGameOver()
{
	bool flag = false;
	//检测是否和地球碰撞了
	KGPoint spos = mSatellite->GetPosition();
	Vector R(spos.x, spos.y);
	float r = R.Module();
	//14为卫星的半径
	if (r < 80 + 14)
		flag = true;

	//检测是否出界
	if (!flag)
	{
		if (spos.x < -386 || spos.x > 386 ||
			spos.y < -286 || spos.y > mCameraLimit * 600 - 14 + 300)
		{
			flag = true;
		}
	}
	//检测是否和别的天体碰撞了
	if (!flag)
	{
		for (auto tmp : mMissionObjectList)
		{
			KGPoint opos = mObject[tmp.id]->GetPosition();
			R = Vector ((opos - spos).x, (opos - spos).y);
			r = R.Module();
			if (r < tmp.radius + 14)
			{
				flag = true;
				break;
			}
		}
	}
	if (flag)
	{
		Sleep(1000);
		iGameInfo->SwitchToScoreScene(mSurveyPoint);
	}
}

void GameScene::CheckSurveyPoint()
{
	//检测是否能获得科研点
	KGPoint spos = mSatellite->GetPosition();
	KGPoint opos = mObject[mTargetInfo.id]->GetPosition();
	Vector R(spos.x - opos.x, spos.y - opos.y);
	float r = R.Module();

	//14为卫星的半径
	if (r < mTargetInfo.surveyRadius + 14)
	{
		mTrueSurveyPoint += 5 * iGameInfo->GetCurApparatus().data / r;
		mSurveyPoint = (int)mTrueSurveyPoint;
		mSurveyPointLabel->SetString(to_wstring(mSurveyPoint));
	}
	
}

void GameScene::SetAimShow(bool isShow)
{
	for (auto tmp : mAimList)
	{
		tmp->SetVisable(isShow);
	}
}

void GameScene::SetRuningShow(bool isShow)
{
	for (auto tmp : mRuningList)
		tmp->SetVisable(isShow);
}

KGPoint GameScene::CalObjectArrowPos()
{
	KGPoint ret(-1000, -2000);
	KGPoint cpos = mCamera.GetCenterPoint();
	KGPoint tpos = mObject[mTargetInfo.id]->GetPosition();
	if (tpos.y - cpos.y > 300)
	{
		ret.y = cpos.y + 300;
		ret.x = tpos.x / (tpos.y - cpos.y) * 300;
	}
	return ret;
}

void GameScene::UpdateAimData()
{
	wchar_t tmp[50];

	swprintf(tmp, L"%3.0f%%", mAimSpeed*100);
	mSpeedLabel->SetString(wstring(tmp));

	swprintf(tmp, L"%3.1f", mAimAngle);
	mAngleLabel->SetString(wstring(tmp));
}
