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
	void UpdateSatellite(float deltaTime);	//��������λ��
	void CheckGameOver();					//����Ƿ���ײ�����
	void CheckSurveyPoint();				//�Ƿ��ܵõ����е�

	void SetAimShow(bool isShow);
	void SetRuningShow(bool isShow);
	KGPoint CalObjectArrowPos();
	void UpdateAimData();
private:
	KGSprite *mObject[8]; //����id = 7
	KGSprite *mObjectArrow;
	KGSprite *mCircle;		//Ŀ��Ȧ
	
	KGLabel *mHelpView;

	KGSprite *mAim;
	KGLabel *mSpeedLabel;
	KGLabel *mAngleLabel;
	vector<KGShaderObject*> mAimList;
	float mAimSpeed;
	float mAimAngle;
	

	GameState mCurState;

	KGSprite *mSatellite; //����
	vector<KGShaderObject*> mRuningList;

	//�������
	int mCameraLimit;
	KGLabel *mSurveyPointLabel;	//̽���
	int mSurveyPoint;
	float mTrueSurveyPoint;

	ObjectInfo mTargetInfo;
	vector<ObjectInfo> mMissionObjectList;

	Vector mSatelliteVelocity;
};

