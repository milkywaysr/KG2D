#include "KGScene.h"
#include "KG2d.h"
#include "KGShaderObject.h"
#include "KGCamera.h"

KGScene::KGScene()
{
}


KGScene::~KGScene()
{
}

void KGScene::AddShaderObject(KGShaderObject * shaderObject)
{
	if (shaderObject)
	{
		mShaderObjectList.push_back(shaderObject);
	}
}

void KGScene::DeleteShanderObjectList()
{
	for (auto &tmp : mShaderObjectList)
	{
		if (tmp)
			delete tmp;
	}
	mShaderObjectList.clear();
}

void KGScene::Update(float deltaTime)
{
}

void KGScene::Draw()
{
	//调用渲染接口
	KG2D::Shader->Render(mShaderObjectList, mCamera);
}
