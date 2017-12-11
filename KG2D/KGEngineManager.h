#pragma once
#include <windows.h>
#include <memory>


class KGScene;
class KGApplication;

class KGEngineManager
{
public:

	KGEngineManager(HINSTANCE hInstance);
	~KGEngineManager();
public:
	void KGEngineStart();			//初始化引擎的函数
	void KGEngineEnd();				//程序结束时调用，释放内存
	void KGLoop();
	void KGMainLoop(float deltaTime);				//引擎主循环
	void KGBeforeEnterMainLoop();	//进入主循环前调用

	int	KGCreateWindow();

	void SetCurrentScene(KGScene * scene);
	void QuitGame();
private:
	KGApplication *mApplication;

	HINSTANCE	mInstance;
	HWND mHwnd;
	
	KGScene *mCurrentScene = nullptr; //当前运行的场景
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
