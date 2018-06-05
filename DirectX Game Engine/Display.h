#include <windows.h>
#include <string>

#pragma once
class Display
{

public:
	Display(HWND hWnd);
	~Display();

	HWND hWnd;

	static Display * CreateDisplay(HINSTANCE hInstance, int nCmdShow, int width, int height, const char * title, const char * className);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

