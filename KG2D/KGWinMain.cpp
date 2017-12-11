#include <windows.h>
#include <d3d11.h>

#include "KGEngineManager.h"
#include "KG2d.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//1 游戏引擎的初始化	EngineStart
	KG2D::EngineManager = make_shared<KGEngineManager>(hInstance);
	KG2D::EngineManager->KGEngineStart();

	//2 游戏的初始化		GameInit
	KG2D::EngineManager->KGBeforeEnterMainLoop();


	//3 游戏循环
	MSG msg;
	while (true)
	{
		//如果windows有消息就处理
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//执行游戏循环
		KG2D::EngineManager->KGLoop();
	}

	//4 游戏结束
	KG2D::EngineManager->KGEngineEnd();
	return 0;
}

