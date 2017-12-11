#include <string>
#include <math.h>
#include <map>
#include <fstream>
#include "GameInfo.h"
#include "StartScene.h"
#include "ReadyScene.h"
#include "GameScene.h"
#include "ScoreScene.h"
using namespace std;

GameInfo* GameInfo::instance = nullptr;

GameInfo::GameInfo()
{
	this->LoadDataFromFile();
	LoadingSave();
	mStartScene = new StartScene();
	mReadyScene = new ReadyScene();
	mGameScene = new GameScene();
	mScoreScene = new ScoreScene();
}


GameInfo::~GameInfo()
{
	delete mStartScene;
	delete mReadyScene;
}

GameInfo * GameInfo::ShareGameInfo()
{
	if (instance == nullptr)
	{
		instance = new GameInfo();
		instance->InitScene();
	}
	return instance;
}

void GameInfo::ReleaseInstance()
{
	if (instance)
		delete instance;
}

void GameInfo::InitScene()
{
	mReadyScene->SetData();
	mGameScene->Init();
	mScoreScene->Init();
}

void GameInfo::NewGame()
{
	//科研点
	mSciencePoint = 0;

	//科技树
	mRocketState[0] = Tech_Select;
	mCurRocket = 0;
	mApparatusState[0] = Tech_Select;
	mCurApparatus = 0;
	for (int i = 1; i < TechSize; i++)
	{
		mRocketState[i] = Tech_Invalid;
		mApparatusState[i] = Tech_Invalid;
	}
	//目标
	int rocketDst = mRocket[mCurRocket].data; //计算火箭距离
	for (int i = 0; i < ObjectSize; i++)
	{
		if (abs(mObjectInfo[i].distance) <= rocketDst)
			mObjectState[i] = Object_Nomal;
		else
			mObjectState[i] = Object_Invalid;

		mSurveyPoint[i] = 0;
	}
	mCurObject = 2; //id2是火星 默认选中
	mObjectState[2] = Object_Select;
}

void GameInfo::LoadingSave()
{
	ifstream file("save");
	if (!file)
	{
		NewGame();
		return;
	}
	file >> mSciencePoint;

	file >> mCurRocket >> mCurApparatus;
	for (int i = 0; i < TechSize; i++)
	{
		int rocket, apparatus;
		file >> rocket >> apparatus;
		mRocketState[i] = (TechState)rocket;
		mApparatusState[i] = (TechState)apparatus;

		if (mRocketState[i] == Tech_Select)
			mCurRocket = i;
		if (mApparatusState[i] == Tech_Select)
			mCurApparatus = i;
	}
	
	file >> mCurObject;
	for (int i = 0; i < ObjectSize; i++)
	{
		int state, point;
		file >> state >> point;
		mObjectState[i] = (ObjectState)state;
		mSurveyPoint[i] = point;
	}
}

void GameInfo::Saving()
{
	ofstream file("save");
	file << mSciencePoint << " ";

	file << mCurRocket << " "; 
	file << mCurApparatus << " ";
	for (int i = 0; i < TechSize; i++)
	{
		file << mRocketState[i]<< " " 
			<< mApparatusState[i] << " ";
	}

	file << mCurObject << " ";
	for (int i = 0; i < ObjectSize; i++)
	{
		file << mObjectState[i] << " ";
		file << mSurveyPoint[i] << " ";
	}
	file.close();
}

void GameInfo::LoadDataFromFile()
{
	//读取科技信息
	wifstream file("resources/tech.txt");
	if (!file)
	{
		MessageBox(NULL, L"XXXXX", L"XXXXXXX", NULL);
	}
	file.imbue(locale("chs"));
	
	while (!file.eof() && file)
	{
		map<wstring, wstring> data;
		for (int i = 0; i < 5; i++)
		{
			wstring tag, value;
			file >> tag >> value;
			data[tag] = value;
		}
		int level = stoi(data[L"level"]);
		if (data[L"type"] == L"rocket")
		{
			mRocket[level].type = Rocket;
			mRocket[level].cost = stoi(data[L"cost"]);
			mRocket[level].name = data[L"name"];
			mRocket[level].data = stoi(data[L"data"]);
		}
		else if (data[L"type"] == L"apparatus")
		{
			mApparatus[level].type = Apparatus;
			mApparatus[level].cost = stoi(data[L"cost"]);
			mApparatus[level].name = data[L"name"];
			mApparatus[level].data = stoi(data[L"data"]);
		}
	}
	file.close();
	//读取天体信息
	file.open("resources/object.txt");
	file.imbue(locale("chs"));

	while (!file.eof() && file)
	{
		map<wstring, wstring> data;
		for (int i = 0; i < 15; i++)
		{
			wstring tag, value;
			file >> tag >> value;
			data[tag] = value;
		}
		int id = stoi(data[L"id"]);
		mObjectInfo[id].id = id;
		mObjectInfo[id].name = data[L"name"];
		mObjectInfo[id].distance = stoi(data[L"dst"]);
		mObjectInfo[id].radius = stof(data[L"r"]);
		mObjectInfo[id].mass = stof(data[L"m"]);
		mObjectInfo[id].surveyRadius = stof(data[L"sr"]);
		mObjectInfo[id].surveyMoney = stoi(data[L"smoney"]);
		for (int i = 0; i < 4; i++)
		{
			wstring tag = L"op" + to_wstring(i);
			mObjectInfo[id].openPoint[i] = stoi(data[tag]);
			tag = L"info" + to_wstring(i);
			mObjectInfo[id].info[i] = data[tag];
		}
	}
	file.close();
	return;
}

void GameInfo::SwitchToStartScene()
{
	KG2D::EngineManager->SetCurrentScene(mStartScene);
}

void GameInfo::SwitchToReadyScene()
{
	mReadyScene->UpdateData();
	KG2D::EngineManager->SetCurrentScene(mReadyScene);
}

void GameInfo::SwitchToGameScene()
{
	mGameScene->CreateMission();
	KG2D::EngineManager->SetCurrentScene(mGameScene);
}

void GameInfo::SwitchToScoreScene(int sp)
{
	mScoreScene->SetData(sp);
	KG2D::EngineManager->SetCurrentScene(mScoreScene);
}

int GameInfo::GetSciencePoint()
{
	return mSciencePoint;
}

TechInfo GameInfo::GetRocketInfoByLevel(int level)
{
	return mRocket[level];
}

TechState GameInfo::GetRocketStateByLevel(int level)
{
	return mRocketState[level];
}

TechInfo GameInfo::GetApparatusInfoByLevel(int level)
{
	return mApparatus[level];
}

TechState GameInfo::GetApparatusStateByLevel(int level)
{
	return mApparatusState[level];
}

TechInfo GameInfo::GetCurRocket()
{
	return mRocket[mCurRocket];
}

TechInfo GameInfo::GetCurApparatus()
{
	return mApparatus[mCurApparatus];
}

void GameInfo::SetCurRocket(int level)
{
	mRocketState[mCurRocket] = Tech_Nomal;
	mCurRocket = level;
	mRocketState[level] = Tech_Select;


	for (int i = 0; i < ObjectSize; i++)
	{
		if (mObjectState[i] == Object_Invalid)
		{
			if (abs(mObjectInfo[i].distance) <= mRocket[level].data)
			{
				mObjectState[i] = Object_Nomal;
			}
		}
		else if (mObjectState[i] == Object_Select)
		{
			if (abs(mObjectInfo[i].distance) > mRocket[level].data)
			{
				//更换火箭后如果射程不够就把目标设置成火星
				mObjectState[i] = Object_Invalid;
				mCurObject = 2;
				mObjectState[2] = Object_Select;
			}
		}
		else
		{
			if (abs(mObjectInfo[i].distance) > mRocket[level].data)
			{
				mObjectState[i] = Object_Invalid;
			}
		}
	}
	mReadyScene->UpdateData();
}

void GameInfo::SetCurApparatus(int level)
{
	mApparatusState[mCurApparatus] = Tech_Nomal;
	mCurApparatus = level;
	mApparatusState[level] = Tech_Select;
	mReadyScene->UpdateData();
}

void GameInfo::ResearchRocket(int level)
{
	mSciencePoint -= mRocket[level].cost;
	mRocketState[level] = Tech_Select;
	mRocketState[mCurRocket] = Tech_Nomal;
	mCurRocket = level;

	if (level + 1 < TechSize && 
		mSciencePoint >= mRocket[level + 1].cost)
	{
		mRocketState[level + 1] = Tech_Research;
	}

	for (int i = 0; i < ObjectSize; i++)
	{
		if (mObjectState[i] == Object_Invalid)
		{
			if (abs(mObjectInfo[i].distance) <= mRocket[level].data)
			{
				mObjectState[i] = Object_Nomal;
			}
		}
	}
	mReadyScene->UpdateData();
}

void GameInfo::ResearchApparatus(int level)
{
	mSciencePoint -= mApparatus[level].cost;
	mApparatusState[level] = Tech_Select;
	mApparatusState[mCurApparatus] = Tech_Nomal;
	mCurApparatus = level;

	if (level + 1 < TechSize &&
		mSciencePoint >= mApparatus[level + 1].cost)
	{
		mApparatusState[level + 1] = Tech_Research;
	}
	mReadyScene->UpdateData();
}

std::wstring GameInfo::GetCurObjectIntroduction()
{
	wstring ret = L"";
	for (int i = 0; i < 4; i++)
	{
		if (mSurveyPoint[mCurObject] >= 
			mObjectInfo[mCurObject].openPoint[i])
		{
			ret += mObjectInfo[mCurObject].info[i];
		}
	}
	return ret;
}

ObjectState GameInfo::GetObjectStateById(int id)
{
	return mObjectState[id];
}

ObjectInfo GameInfo::GetObjectInfoById(int id)
{
	return mObjectInfo[id];
}

ObjectInfo GameInfo::GetCurObject()
{
	return mObjectInfo[mCurObject];
}

int GameInfo::GetCurObjectSurveyLevel()
{
	for (int i = 0; i < 4; i++)
	{
		if (mSurveyPoint[mCurObject] <
			mObjectInfo[mCurObject].openPoint[i])
		{
			return i;
		}
	}
	return 1;
}

void GameInfo::SetCurObject(int id)
{
	mObjectState[id] = Object_Select;
	mObjectState[mCurObject] = Object_Nomal;
	mCurObject = id;
	mReadyScene->UpdateData();
}

void GameInfo::ProcessSurveyResult(int sp, int money)
{
	mSurveyPoint[mCurObject] += sp;
	mSciencePoint += money;

	for (int i = 0; i < TechSize; i++)
	{
		if (mRocketState[i] == Tech_Invalid)
		{
			if (mRocket[i].cost <= mSciencePoint)
			{
				mRocketState[i] = Tech_Research;
			}
			break;
		}
	}

	for (int i = 0; i < TechSize; i++)
	{
		if (mApparatusState[i] == Tech_Invalid)
		{
			if (mApparatus[i].cost <= mSciencePoint)
			{
				mApparatusState[i] = Tech_Research;
			}
			break;
		}
	}
}
