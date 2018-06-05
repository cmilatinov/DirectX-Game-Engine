#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define VK_CTRL					0x1
#define VK_SHIFT				0x2
#define VK_CTRL_SHIFT			0x3

#define VK_RELEASE				0x4
#define VK_PRESS				0x5

#define VM_RMB					0x6
#define VM_LMB					0x7
#define VM_MMB					0x8


#pragma once
class Input
{
private:
	IDirectInput8 *				directInput;
	IDirectInputDevice8 *		keyboard;
	IDirectInputDevice8 *		mouse;

	BYTE currentKeyboardState[256], oldKeyboardState[256];
	DIMOUSESTATE mouseState;

	int screenWidth, screenHeight;
	int mouseX, mouseY;

	void ReadKeyboard();
	void ReadMouse();

	void (*keyCallback) (int type, BYTE key, int modifiers);
	void (*mouseCallback) (int type, int x, int y, int button);

public:
	Input(HINSTANCE hInstance, HWND hWnd, int screenWidth, int screenHeight);
	~Input();

	void Frame(double deltaSec);

	void SetKeyEventCallback(void (*callback)(int type, BYTE key, int modifiers));
};

