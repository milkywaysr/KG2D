#include "KGInput.h"


KGInput::KGInput(HINSTANCE hInstance, HWND hwnd):
mHwnd(hwnd),
mMouse(nullptr),
mKeyboard(nullptr),
mMPosX(0),
mMPosY(0)
{
	memset(&mMouseState, 0, sizeof(mMouseState));
	memset(&mOldMouseState, 0, sizeof(mOldMouseState));
	memset(&mKeys, 0, sizeof(mKeys));
	memset(&mOldKeys, 0, sizeof(mOldKeys));

	HRESULT hr;
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&mInputSystem, NULL);
	//初始化键盘
	hr = mInputSystem->CreateDevice(GUID_SysKeyboard, &mKeyboard, NULL);
	hr = mKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = mKeyboard->SetCooperativeLevel(mHwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	hr = mKeyboard->Acquire();
	memset(mKeys, 0, sizeof(mKeys));
	memset(mOldKeys, 0, sizeof(mOldKeys));
	//初始化鼠标
	hr = mInputSystem->CreateDevice(GUID_SysMouse, &mMouse, NULL);
	hr = mMouse->SetDataFormat(&c_dfDIMouse);
	hr = mMouse->SetCooperativeLevel(mHwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	hr = mMouse->Acquire();
	POINT pos = { 0, 0 };
	
	mMPosX = pos.x;
	mMPosY = pos.y;
}


KGInput::~KGInput()
{
	mKeyboard->Unacquire();
	mKeyboard->Release();
	mKeyboard = NULL;

	mMouse->Unacquire();
	mMouse->Release();
	mMouse = NULL;

	mInputSystem->Release();
	mInputSystem = NULL;
}

bool KGInput::UpdateDevices()
{
	HRESULT hr;
	//更新鼠标状态
	memcpy(&mOldMouseState, &mMouseState, sizeof(mMouseState));
	hr = mMouse->Acquire();
	hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mMouseState);

	//更新键盘状态
	memcpy(&mOldKeys, &mKeys, sizeof(mKeys));
	hr = mKeyboard->Acquire();
	hr = mKeyboard->GetDeviceState(sizeof(mKeys), (LPVOID)mKeys);

	return true;
}

bool KGInput::IsKeyDown(unsigned int key)
{
	return mKeys[key] & 0x80;
}

bool KGInput::IsKeyUp(unsigned int key)
{
	return (!(mKeys[key] & 0x80) && 
		mKeys[key] != mOldKeys[key]);
}

bool KGInput::IsMouseLeftKeyDown()
{
	return mMouseState.rgbButtons[0] & 0x80;
}

bool KGInput::IsMouseLeftKeyUp()
{
	return (!(mMouseState.rgbButtons[0] & 0x80) &&
		mMouseState.rgbButtons[0] !=
		mOldMouseState.rgbButtons[0]);
}

KGPoint KGInput::GetMousePosition()
{
	KGPoint ret;
	POINT pos = { mMPosX, mMPosY};
	GetCursorPos(&pos);
	ScreenToClient(mHwnd, &pos);
	ret.x = pos.x;
	ret.y = pos.y;
	return ret;
}

