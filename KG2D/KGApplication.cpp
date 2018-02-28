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
}

void KGApplication::KGApplicationMainLoop()
{
	//游戏的主循环
	
}

void KGApplication::KGApplicationExit()
{
	//退出游戏，释放资源
	
}
