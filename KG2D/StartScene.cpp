#include "StartScene.h"
#include "GameInfo.h"


StartScene::StartScene()
{
	KGColorRGB labelColor = { 44, 150, 180 };
	mStartGame = new KGLabel(L"开始游戏", 30, labelColor);
	mStartGame->SetPosition(KGPoint(350, 400));
	AddShaderObject(mStartGame);

	mExitGame = new KGLabel(L"退出游戏", 30, labelColor);
	mExitGame->SetPosition(KGPoint(350, 450));
	AddShaderObject(mExitGame);

	KGSprite *bg = new KGSprite(wstring(L"resources/start_bg.dds"));
	bg->SetPosition(KGPoint(-400, -300));
	AddShaderObject(bg);
}


StartScene::~StartScene()
{
	this->DeleteShanderObjectList();
}

void StartScene::Update(float deltaTime)
{
	KGPoint pos = KG2D::Input->GetMousePosition();
	
	if (KG2D::Input->IsMouseLeftKeyUp())
	{
		if (CheckLabelHit(pos, *mStartGame))
		{
			//MessageBox(NULL, L"StartGame", L"StartGame", NULL);
			GameInfo::ShareGameInfo()->SwitchToReadyScene();
		}
		if (CheckLabelHit(pos, *mExitGame))
		{
			KG2D::EngineManager->QuitGame();
		}
	}
}
