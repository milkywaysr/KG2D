#include <iostream>
#include "KGApplication.h"
#include "KGSprite.h"
#include "KGLabel.h"
#include "KG2DTool.h"

KGApplication::KGApplication()
{
}


KGApplication::~KGApplication()
{

}

void KGApplication::KGApplicationInit()
{
	//游戏初始化
	mScene = new KGScene();
	KG2D::EngineManager->SetCurrentScene(mScene);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			mLabel = new KGLabel(L"测试", 20, { 255, 255, 255 }, KGPoint(i * 50 + 100, j * 50));
			mScene->AddShaderObject(mLabel);
		}
	}
}

void KGApplication::KGApplicationMainLoop()
{
	//游戏的主循环
	//mScene->DeleteShanderObjectList();
	
}

void KGApplication::KGApplicationExit()
{
	//退出游戏，释放资源
	mScene->DeleteShanderObjectList();
}
