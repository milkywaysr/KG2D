#pragma once
#include <dinput.h>
#include "KGPoint.h"

#define KEYS_SIZE 256


class KGInput
{
public:
	KGInput(HINSTANCE hInstance, HWND hwnd);
	~KGInput();
	//更新设备状态
	bool UpdateDevices();
	//键盘函数
	bool IsKeyDown(unsigned int key);
	bool IsKeyUp(unsigned int key);
	//鼠标函数
	bool IsMouseLeftKeyDown();
	bool IsMouseLeftKeyUp();
	KGPoint GetMousePosition();

private:
	HWND mHwnd;
	LPDIRECTINPUT8 mInputSystem;

	//键盘设备
	LPDIRECTINPUTDEVICE8 mKeyboard;
	char mKeys[KEYS_SIZE];
	char mOldKeys[KEYS_SIZE];

	//鼠标设备
	LPDIRECTINPUTDEVICE8 mMouse;
	DIMOUSESTATE mMouseState;
	DIMOUSESTATE mOldMouseState;
	long mMPosX;
	long mMPosY;
};

