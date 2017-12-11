#pragma once
#include <dinput.h>
#include "KGPoint.h"

#define KEYS_SIZE 256


class KGInput
{
public:
	KGInput(HINSTANCE hInstance, HWND hwnd);
	~KGInput();
	//�����豸״̬
	bool UpdateDevices();
	//���̺���
	bool IsKeyDown(unsigned int key);
	bool IsKeyUp(unsigned int key);
	//��꺯��
	bool IsMouseLeftKeyDown();
	bool IsMouseLeftKeyUp();
	KGPoint GetMousePosition();

private:
	HWND mHwnd;
	LPDIRECTINPUT8 mInputSystem;

	//�����豸
	LPDIRECTINPUTDEVICE8 mKeyboard;
	char mKeys[KEYS_SIZE];
	char mOldKeys[KEYS_SIZE];

	//����豸
	LPDIRECTINPUTDEVICE8 mMouse;
	DIMOUSESTATE mMouseState;
	DIMOUSESTATE mOldMouseState;
	long mMPosX;
	long mMPosY;
};

