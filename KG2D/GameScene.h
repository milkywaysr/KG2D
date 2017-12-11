#pragma once
#include <vector>
#include "KG2d.h"
#include "GameInfo.h"
#include "Vector.h"
using namespace std;

enum GameState {Game_View, Game_Aim, Game_Runing};
class GameScene :
	public KGScene
{
public:
	GameScene();
	~GameScene();
	void Update(float deltaTime);

	void Init();
	void CreateMission();
private:
	void OnView();
	void OnAim();
	void OnRuning(float deltaTime);

	void BeforeRuning();
	void UpdateSatellite(float deltaTime);	//更新卫星位置
	void CheckGameOver();					//检测是否碰撞或出界
	void CheckSurveyPoint();				//是否能得到科研点

	void SetAimShow(bool isShow);
	void SetRuningShow(bool isShow);
	KGPoint CalObjectArrowPos();
	void UpdateAimData();
private:
	KGSprite *mObject[8]; //地球id = 7
	KGSprite *mObjectArrow;
	KGSprite *mCircle;		//目标圈
	
	KGLabel *mHelpView;

	KGSprite *mAim;
	KGLabel *mSpeedLabel;
	KGLabel *mAngleLabel;
	vector<KGShaderObject*> mAimList;
	float mAimSpeed;
	float mAimAngle;
	

	GameState mCurState;

	KGSprite *mSatellite; //卫星
	vector<KGShaderObject*> mRuningList;

	//任务相关
	int mCameraLimit;
	KGLabel *mSurveyPointLabel;	//探测点
	int mSurveyPoint;
	float mTrueSurveyPoint;

	ObjectInfo mTargetInfo;
	vector<ObjectInfo> mMissionObjectList;

	Vector mSatelliteVelocity;
};

