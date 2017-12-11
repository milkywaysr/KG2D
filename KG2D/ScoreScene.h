#pragma once
#include "KG2d.h"
class ScoreScene :
	public KGScene
{
public:
	ScoreScene();
	~ScoreScene();

	void Update(float deltaTime);

	void Init();
	void SetData(int surveyPoint);
	
private:
	KGLabel *mResult;
	KGLabel *mReturn;

	int mSurveyPoint;
	int mMoney;
};