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
	//��Ϸ��ʼ��
	mScene = new KGScene();
	KG2D::EngineManager->SetCurrentScene(mScene);
}

void KGApplication::KGApplicationMainLoop()
{
	//��Ϸ����ѭ��
	
}

void KGApplication::KGApplicationExit()
{
	//�˳���Ϸ���ͷ���Դ
	
}
