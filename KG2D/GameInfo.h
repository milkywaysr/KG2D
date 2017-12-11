#pragma once
#include <string>

enum TechType { Rocket, Apparatus }; //火箭和仪器
enum TechState { Tech_Nomal, Tech_Select, Tech_Research, Tech_Invalid};
struct TechInfo
{
	TechType type;	//科技类型：火箭、仪器
	std::wstring name;	//科技名称
	int cost;		//升级经费
	int data;		//科技性能：发射距离、收集速率
};

enum ObjectState {Object_Nomal, Object_Select, Object_Invalid};
struct ObjectInfo
{
	int id;
	std::wstring name;		//名字
	int distance;			//距离
	float radius;			//半径
	float mass;				//质量
	float surveyRadius;		//探测半径
	int surveyMoney;		//经费系数
	int openPoint[4];		//简介开放等级
	std::wstring info[4];	//简介
};

class StartScene;
class ReadyScene;
class GameScene;
class ScoreScene;
class GameInfo
{
public:
	GameInfo();
	~GameInfo();

	static GameInfo* ShareGameInfo();
	static void ReleaseInstance();
	void InitScene();

	void NewGame();
	void LoadingSave();
	void Saving();
	void LoadDataFromFile();

	void SwitchToStartScene();
	void SwitchToReadyScene();
	void SwitchToGameScene();
	void SwitchToScoreScene(int sp);

	int GetSciencePoint();
	//科技树
	TechInfo GetRocketInfoByLevel(int level);
	TechState GetRocketStateByLevel(int level);
	TechInfo GetApparatusInfoByLevel(int level);
	TechState GetApparatusStateByLevel(int level);
	TechInfo GetCurRocket();
	TechInfo GetCurApparatus();

	void SetCurRocket(int level);
	void SetCurApparatus(int level);

	void ResearchRocket(int level);
	void ResearchApparatus(int level);

	//目标选择
	std::wstring GetCurObjectIntroduction();
	ObjectState GetObjectStateById(int id);
	ObjectInfo GetObjectInfoById(int id);
	ObjectInfo GetCurObject();
	int GetCurObjectSurveyLevel();

	void SetCurObject(int id);

	void ProcessSurveyResult(int sp, int money);
private:
	//场景
	StartScene *mStartScene;
	ReadyScene *mReadyScene;
	GameScene *mGameScene;
	ScoreScene *mScoreScene;

	//科技
	static const int TechSize = 4;
	int mCurRocket;						//当前火箭
	TechInfo mRocket[TechSize];			//火箭信息
	TechState mRocketState[TechSize];	//火箭状态

	int mCurApparatus;					//当前仪器
	TechInfo mApparatus[TechSize];		//仪器信息
	TechState mApparatusState[TechSize];//仪器状态

	//天体
	static const int ObjectSize = 7;
	int mCurObject;							//当前天体
	ObjectInfo mObjectInfo[ObjectSize];		//天体信息
	ObjectState mObjectState[ObjectSize];	//天体状态
	int mSurveyPoint[ObjectSize];			//天体探测点数

	//科研点
	int mSciencePoint;

	static GameInfo *instance;
};
#define iGameInfo GameInfo::ShareGameInfo()