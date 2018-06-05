#include <iostream>
#include <chrono>

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include <d3d11.h>
#include <d3dx9math.h>
#include <d3dx10.h>
#include <d3dx11.h>

#include "Display.h"
#include "Engine.h"
#include "Input.h"

#define __CONSOLE_OUTPUT__

static int SCREEN_WIDTH = 1280;
static int SCREEN_HEIGHT = 720;

#pragma comment(lib, "version")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dx9")
#pragma comment(lib, "d3dx10")
#pragma comment(lib, "d3dx11")
#pragma comment(lib, "GLMath")

using namespace std;

void InitConsole();
bool InitD3D(HWND hWnd);
bool CreateDeviceAndSwapChain(HWND hWnd);
bool CreateBuffers();
void CleanD3D();
void KeyEvent(int type, int key, int modifiers);

void gotoxy(int x, int y);
void print_video_card_info(ID3D11Device *  device);
void print_windows_version();
void get_screen_size();

Engine *						engine;
Input *							input;

IDXGISwapChain *				swapchain;
ID3D11Device *					device;
ID3D11DeviceContext *			devcon;

ID3D11RenderTargetView *		backbuffer;
ID3D11DepthStencilView *		depthbuffer;

int fps = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	// show the console
#ifdef __CONSOLE_OUTPUT__
	InitConsole();
#endif 
	
	gotoxy(0, 0);
	print_windows_version();

	gotoxy(0, 1);
	get_screen_size();
	cout << "Screen Size : " << SCREEN_WIDTH << " x " << SCREEN_HEIGHT << endl;
	
	// create display
	Display * display = Display::CreateDisplay(hInstance, nCmdShow, 1280, 720, "DirectX11 Game Engine", "WindowClass1");

	// create device, device context, and swapchain
	if (!InitD3D(display->hWnd)) {
		MessageBox(NULL, "Direct3D initialization failed", "Error", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}

	gotoxy(0, 2);
	print_video_card_info(device);
	
	// create the engine
	engine = new Engine(device, devcon, swapchain, backbuffer, depthbuffer);
	
	// create the input
	input = new Input(hInstance, display->hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);
	input->SetKeyEventCallback(KeyEvent);

	// initialize the engine
	if (!engine->InitPipeline()) {
		MessageBox(NULL, "Engine pipeline initialization failed", "Error", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}

	// this struct holds Windows event messages
	MSG msg;

	// this is to calculate the time between frames
	auto lastFrame = chrono::high_resolution_clock::now();
	auto lastSec = chrono::high_resolution_clock::now();
	
	// wait for the next message in the queue, store the result in 'msg'
	while (true)
	{	
		// as opposed to GetMessage, PeekMessage checks if there is a message in the queue to be processed
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);

			// check to see if it's time to quit
			if (msg.message == WM_QUIT)
				break;

		}
		else {
			
			// game code
			auto currentFrame = chrono::high_resolution_clock::now();
			double deltaSec = chrono::duration_cast<chrono::nanoseconds>(currentFrame - lastFrame).count() / 10e+9;

			// render the next frame
			input->Frame(deltaSec);
			engine->RenderFrame(deltaSec);
			fps++;
			
			auto currentSec = chrono::high_resolution_clock::now();
			double deltaTime = chrono::duration_cast<chrono::seconds>(currentSec - lastSec).count();
			if (deltaTime >= 1.0) {
				gotoxy(0, 3);
				cout << "FPS: " << fps << endl;
				fps = 0;
				lastSec = currentSec;
			}

			// reset the frame timer
			lastFrame = currentFrame;

		}
	}

	// clean up DirectX and COM
	engine->Cleanup();
	CleanD3D();

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

void InitConsole() {

	AllocConsole();
	HANDLE stdHandle;
	int hConsole;
	FILE* fp;
	stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	hConsole = _open_osfhandle((long)stdHandle, _O_TEXT);
	fp = _fdopen(hConsole, "w");

	freopen_s(&fp, "CONOUT$", "w", stdout);

}

bool InitD3D(HWND hWnd) {

	if (!CreateDeviceAndSwapChain(hWnd)){
		MessageBox(NULL, "Failed to create Direct3D device", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	if(!CreateBuffers())
		return false;
	
	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	devcon->RSSetViewports(1, &viewport);

	return true;
}

bool CreateDeviceAndSwapChain(HWND hWnd) {
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(scd));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH;                    // set the back buffer width
	scd.BufferDesc.Height = SCREEN_HEIGHT;                  // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 8;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

															// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&device,
		NULL,
		&devcon);

	if (FAILED(hr))
		return false;
	else
		return true;
}

bool CreateBuffers() {

	// get the address of the back buffer
	ID3D11Texture2D * pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	HRESULT hr = device->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create back buffer", "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	pBackBuffer->Release();

	// depth stencil texture
	ID3D11Texture2D * pDepthStencil;

	// texture settings
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = SCREEN_WIDTH;
	descDepth.Height = SCREEN_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 8;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	// create the texture
	hr = device->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create depth texture", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));

	// depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// set the depth stencil buffer settings
	ID3D11DepthStencilState * pDSState;
	hr = device->CreateDepthStencilState(&dsDesc, &pDSState);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create depth-stencil state", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	devcon->OMSetDepthStencilState(pDSState, 1);

	// create depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(pDepthStencil, &descDSV, &depthbuffer);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create depth-stencil view", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, depthbuffer);
	
	return true;
}

void CleanD3D() {

	// switch to windowed mode
	swapchain->SetFullscreenState(FALSE, NULL);

	// release components
	swapchain->Release();
	device->Release();
	devcon->Release();
	backbuffer->Release();
	depthbuffer->Release();
	
}

void KeyEvent(int type, int key, int modifiers) {

	gotoxy(0, 4);
	cout << "                                                                             ";
	gotoxy(0, 4);
	cout << "Key '" << (int)key << "' was " << (type == VK_PRESS ? "pressed" : "released") << " with modifier " << modifiers << endl;

}



// utilities
void gotoxy(int x, int y){

	static HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);

}

void print_video_card_info(ID3D11Device * device){
	
	IDXGIDevice * pDXGIDevice;
	device->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);

	IDXGIAdapter * pDXGIAdapter;
	pDXGIDevice->GetAdapter(&pDXGIAdapter);

	DXGI_ADAPTER_DESC desc;
	pDXGIAdapter->GetDesc(&desc);

	wcout << "Video Card : " << desc.Description << endl;

}

void print_windows_version(){
	
	DWORD buffer_size = GetFileVersionInfoSize("kernel32.dll", NULL);
	if (buffer_size == 0){
		// get error from GetLastError()
		return;
	}

	VOID *buffer = malloc(buffer_size);
	if (buffer == NULL){
		// out of memory
		free(buffer);
		return;
	}

	if (!GetFileVersionInfo("kernel32.dll", 0, buffer_size, buffer)){
		free(buffer);
		return;
	}

	VS_FIXEDFILEINFO *version = NULL;
	UINT version_len = 0;
	if (!VerQueryValue(buffer, "\\", (LPVOID*)&version, &version_len)){
		free(buffer);
		return;
	}

	printf("Windows Version : %u.%u.%u \n",
		HIWORD(version->dwProductVersionMS),
		LOWORD(version->dwProductVersionMS),
		HIWORD(version->dwProductVersionLS));

	free(buffer);
	
}

void get_screen_size(){

	RECT rect;
	GetWindowRect(GetDesktopWindow(), &rect);
	SCREEN_WIDTH = rect.right - rect.left;
	SCREEN_HEIGHT = rect.bottom - rect.top;

}
