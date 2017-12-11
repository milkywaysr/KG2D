#pragma once
#include "KG2d.h"

class StartScene :
	public KGScene
{
public:
	StartScene();
	~StartScene();

	void Update(float deltaTime);
private:
	KGLabel *mStartGame;
	KGLabel *mExitGame;
};

