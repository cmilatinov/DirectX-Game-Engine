#include "Display.h"
#include <iostream>


Display::Display(HWND hWnd){
	this->hWnd = hWnd;
}


Display::~Display(){

}

Display * Display::CreateDisplay(HINSTANCE hInstance, int nCmdShow, int width, int height, const char * title, const char * className){
	
	//Window handle
	HWND hWnd;
	
	//Window class struct
	WNDCLASSEX wc;

	//If class doesnt exist
	if (!GetClassInfoEx(hInstance, className, &wc)) {

		//Zero the memory of the struct
		ZeroMemory(&wc, sizeof(wc));

		//Window class params
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInstance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.lpszClassName = className;

		//Register the class
		RegisterClassEx(&wc);

	}

	//Center the window based on desktop
	RECT rect;
	GetClientRect(GetDesktopWindow(), &rect);
	rect.left = (rect.right / 2) - (width / 2);
	rect.top = (rect.bottom / 2) - (height / 2);

	RECT wr = { 0, 0, width - 50, height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	//Create the window
	hWnd = CreateWindowEx(	NULL,					//Extension of style
							className,				//Window class name
							title,					//Window name
							WS_OVERLAPPEDWINDOW,	//Style 
							rect.left,				//X coordinate
							rect.top,				//Y coordinate
							width,					//Width
							height,					//Height 
							NULL,					//Parent Window
							NULL,					//Menu(s)
							hInstance,				//Instance handle 
							NULL	);				//Used for multiple windows

	//Show the window
	ShowWindow(hWnd, nCmdShow);

	//Return a display object
	return new Display(hWnd);
}

LRESULT Display::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

	// sort through and find what code to run for the message given
	switch (message)
	{
		
		// this message is read when the window is closed
		case WM_DESTROY:
		{
			// close the application entirely
			PostQuitMessage(0);
			return 0;
		} 
		
		break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}
