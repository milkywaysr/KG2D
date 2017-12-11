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
	void KGEngineStart();			//��ʼ������ĺ���
	void KGEngineEnd();				//�������ʱ���ã��ͷ��ڴ�
	void KGLoop();
	void KGMainLoop(float deltaTime);				//������ѭ��
	void KGBeforeEnterMainLoop();	//������ѭ��ǰ����

	int	KGCreateWindow();

	void SetCurrentScene(KGScene * scene);
	void QuitGame();
private:
	KGApplication *mApplication;

	HINSTANCE	mInstance;
	HWND mHwnd;
	
	KGScene *mCurrentScene = nullptr; //��ǰ���еĳ���
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
