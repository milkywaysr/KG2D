#pragma once
#include <list>
#include "KGShaderObject.h"
#include "KGCamera.h"
using namespace std;

class KGScene
{
public:
	KGScene();
	virtual ~KGScene();

	void AddShaderObject(KGShaderObject*);
	void DeleteShanderObjectList();			//释放渲染列表中所有元素
	virtual void Update(float deltaTime); //游戏逻辑更新
	void Draw();	//绘制场景
protected:
	list<KGShaderObject*> mShaderObjectList; //渲染列表
	KGCamera mCamera;
};

