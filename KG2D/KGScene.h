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
	void DeleteShanderObjectList();			//�ͷ���Ⱦ�б�������Ԫ��
	virtual void Update(float deltaTime); //��Ϸ�߼�����
	void Draw();	//���Ƴ���
protected:
	list<KGShaderObject*> mShaderObjectList; //��Ⱦ�б�
	KGCamera mCamera;
};

