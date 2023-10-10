#pragma once
#include <Windows.h>

#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <wrl.h>

//using namespace Microsoft::WRL;

class Input
{
public:
	//
	void Initialize(HINSTANCE hInstance, HWND hwnd);
	//
	void Update();

private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
};

