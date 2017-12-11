#include "KGEngineManager.h"
#include <functional>
#include <sstream>
#include <objidl.h>
#include <gdiplus.h>
#include "KG2d.h"
#include "KGTimer.h"
#include "KGShaderInterface.h"
#include "KGShaderFactory.h"
#include "KGScheduler.h"
#include "KGInput.h"
#include "KGScene.h"
#include "KGApplication.h"


using namespace Gdiplus;

KGEngineManager::KGEngineManager(HINSTANCE hInstance) :
mInstance(hInstance),
mApplication(nullptr)
{
}

KGEngineManager::~KGEngineManager()
{
}

void KGEngineManager::KGEngineStart()
{
	//��start���洴�������Կ��Ƹ������ʼ����˳��
	//ʱ�䲿����ʼ��
	KG2D::Timer = std::make_shared<KGTimer>();
	//��������ʾ����
	KGCreateWindow();
	//��ʼ����Ⱦ����
	KG2D::Shader = KGShaderFactory::GetShaderInterface(mHwnd, DirectX11Shader);
	//��ʼ�������豸
	KG2D::Input = std::make_shared<KGInput>(mInstance, mHwnd);
}

void KGEngineManager::KGEngineEnd()
{
	mApplication->KGApplicationExit();
	delete mApplication;
	KG2D::Timer.reset();
	KG2D::Shader.reset();
}

void KGEngineManager::KGLoop()
{
	KG2D::Timer->Tick();
}

void KGEngineManager::KGMainLoop(float deltaTime)
{
	KG2D::Input->UpdateDevices();
	mApplication->KGApplicationMainLoop();
	g_fps = 1.0 / deltaTime;
	if (mCurrentScene)
	{
		mCurrentScene->Update(deltaTime);
		mCurrentScene->Draw();
	}
}

void KGEngineManager::KGBeforeEnterMainLoop()
{
	KG2D::Timer->Reset();		//����ʱ��
	mApplication = new KGApplication();
	mApplication->KGApplicationInit();
	KG2D::Timer->RegisterScheduler(MemberFunScheduler(KGMainLoop, 60, -1));
}

int KGEngineManager::KGCreateWindow()
{
	//
	//��������
	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRectEx(&rc, WS_POPUPWINDOW | WS_CAPTION, FALSE, NULL);
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = NULL;
	wcex.cbWndExtra = NULL;
	wcex.hInstance = mInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"wcex";
	wcex.hIconSm = NULL;

	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	mHwnd = CreateWindow(L"wcex", L"̫�ռƻ�", WS_POPUPWINDOW | WS_CAPTION,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right-rc.left, rc.bottom - rc.top, NULL, NULL, mInstance, NULL);

	if (!mHwnd)
		return E_FAIL;

	ShowWindow(mHwnd, SW_SHOW);

	return 0;
}

void KGEngineManager::SetCurrentScene(KGScene * scene)
{
	mCurrentScene = scene;
}

void KGEngineManager::QuitGame()
{
	PostQuitMessage(0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}