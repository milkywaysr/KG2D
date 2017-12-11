#pragma once
//    引擎提供给游戏的接口		//
//	  自定义游戏各个阶段的行为	//
//								//

class KGApplication
{
public:
	KGApplication();
	~KGApplication();

public:
	void KGApplicationInit();
	void KGApplicationMainLoop();
	void KGApplicationExit();
	
private:
};

