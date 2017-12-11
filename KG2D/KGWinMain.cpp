#include <windows.h>
#include <d3d11.h>

#include "KGEngineManager.h"
#include "KG2d.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//1 ��Ϸ����ĳ�ʼ��	EngineStart
	KG2D::EngineManager = make_shared<KGEngineManager>(hInstance);
	KG2D::EngineManager->KGEngineStart();

	//2 ��Ϸ�ĳ�ʼ��		GameInit
	KG2D::EngineManager->KGBeforeEnterMainLoop();


	//3 ��Ϸѭ��
	MSG msg;
	while (true)
	{
		//���windows����Ϣ�ʹ���
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//ִ����Ϸѭ��
		KG2D::EngineManager->KGLoop();
	}

	//4 ��Ϸ����
	KG2D::EngineManager->KGEngineEnd();
	return 0;
}

