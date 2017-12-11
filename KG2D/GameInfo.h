#pragma once
#include <string>

enum TechType { Rocket, Apparatus }; //���������
enum TechState { Tech_Nomal, Tech_Select, Tech_Research, Tech_Invalid};
struct TechInfo
{
	TechType type;	//�Ƽ����ͣ����������
	std::wstring name;	//�Ƽ�����
	int cost;		//��������
	int data;		//�Ƽ����ܣ�������롢�ռ�����
};

enum ObjectState {Object_Nomal, Object_Select, Object_Invalid};
struct ObjectInfo
{
	int id;
	std::wstring name;		//����
	int distance;			//����
	float radius;			//�뾶
	float mass;				//����
	float surveyRadius;		//̽��뾶
	int surveyMoney;		//����ϵ��
	int openPoint[4];		//��鿪�ŵȼ�
	std::wstring info[4];	//���
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
	//�Ƽ���
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

	//Ŀ��ѡ��
	std::wstring GetCurObjectIntroduction();
	ObjectState GetObjectStateById(int id);
	ObjectInfo GetObjectInfoById(int id);
	ObjectInfo GetCurObject();
	int GetCurObjectSurveyLevel();

	void SetCurObject(int id);

	void ProcessSurveyResult(int sp, int money);
private:
	//����
	StartScene *mStartScene;
	ReadyScene *mReadyScene;
	GameScene *mGameScene;
	ScoreScene *mScoreScene;

	//�Ƽ�
	static const int TechSize = 4;
	int mCurRocket;						//��ǰ���
	TechInfo mRocket[TechSize];			//�����Ϣ
	TechState mRocketState[TechSize];	//���״̬

	int mCurApparatus;					//��ǰ����
	TechInfo mApparatus[TechSize];		//������Ϣ
	TechState mApparatusState[TechSize];//����״̬

	//����
	static const int ObjectSize = 7;
	int mCurObject;							//��ǰ����
	ObjectInfo mObjectInfo[ObjectSize];		//������Ϣ
	ObjectState mObjectState[ObjectSize];	//����״̬
	int mSurveyPoint[ObjectSize];			//����̽�����

	//���е�
	int mSciencePoint;

	static GameInfo *instance;
};
#define iGameInfo GameInfo::ShareGameInfo()