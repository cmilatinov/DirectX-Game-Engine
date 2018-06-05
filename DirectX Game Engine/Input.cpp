#include "Input.h"
#include <iostream>

Input::Input(HINSTANCE hInstance, HWND hWnd, int screenWidth, int screenHeight){
	
	HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create DirectInput instance", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create keyboard device", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to set keyboard data format", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	hr = keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to set keyboard cooperative level", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	hr = keyboard->Acquire();
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to acquire keyboard device", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	hr = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create keyboard device", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	hr = mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to set mouse data format", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	hr = mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to set mouse cooperative level", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	hr = mouse->Acquire();
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to acquire mouse device", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

}	

Input::~Input(){
	mouse->Unacquire();
	mouse->Release();

	keyboard->Unacquire();
	keyboard->Release();

	directInput->Release();
}

void Input::Frame(double deltaSec) {
	
	ReadKeyboard();
	ReadMouse();

	if (currentKeyboardState[DIK_ESCAPE]) {
		std::cout << ((int)currentKeyboardState[DIK_ESCAPE]) << std::endl;
		system("pause");
		exit(EXIT_SUCCESS);
	}

	int currentModifiers = 0;
	if (currentKeyboardState[DIK_LCONTROL] || currentKeyboardState[DIK_RCONTROL])
		currentModifiers |= VK_CTRL;
	if (currentKeyboardState[DIK_LSHIFT] || currentKeyboardState[DIK_RSHIFT])
		currentModifiers |= VK_SHIFT;

	if (keyCallback == nullptr)
		return;

	for (int c = 0; c < sizeof(currentKeyboardState); c++) {
		if (currentKeyboardState[c] != oldKeyboardState[c]) {
			if (currentKeyboardState[c])
				keyCallback(VK_PRESS, currentKeyboardState[c], currentModifiers);
			else
				keyCallback(VK_RELEASE, currentKeyboardState[c], currentModifiers);
		}
	}

	mouseState.rgbButtons;
}

void Input::ReadKeyboard() {
	
	for (int c = 0; c < sizeof(currentKeyboardState); c++) 
		oldKeyboardState[c] = currentKeyboardState[c];
	
	HRESULT hr = keyboard->GetDeviceState(sizeof(currentKeyboardState), (LPVOID)&currentKeyboardState);
	if (FAILED(hr)) {
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
			keyboard->Acquire();
	}

}

void Input::ReadMouse() {
	
	HRESULT hr = mouse->GetDeviceState(sizeof(mouseState), (LPVOID)&mouseState);
	if (FAILED(hr)) {
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
			mouse->Acquire();
	}

}

void Input::SetKeyEventCallback(void(*callback)(int type, BYTE key, int modifiers)) {

	this->keyCallback = callback;

}
