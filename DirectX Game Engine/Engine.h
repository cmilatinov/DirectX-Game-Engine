#include <iostream>
#include <GLMatrix.h>

#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>

#include "StaticShader.h"
#include "Mesh.h"
#include "Loader.h"
#include "Camera.h"

#pragma once
class Engine
{
private:
	IDXGISwapChain *			swapchain;
	ID3D11Device *				device; 
	ID3D11DeviceContext *		devcon; 
	
	ID3D11RenderTargetView *	backbuffer;
	ID3D11DepthStencilView *	depthbuffer;
	
	ID3D11SamplerState *		samplerState;
	ID3D11RasterizerState *		rasterizerState;

	ID3D11Buffer *				pProjMatBuffer;
	ID3D11Buffer *				pViewMatBuffer;
	ID3D11Buffer *				pConstBuffer;
	ID3D11Buffer **				buffers;

	Loader *					loader;

	//=============================TEMP=================================
	StaticShader *				shader;
	Mesh *						mesh;
	Texture *					texture;
	Camera *					camera;
	//==================================================================

public:
	struct UNIFORMS {
		glmath::mat4 modelMatrix;
	};
	UNIFORMS uniforms;

	//=============================TEMP=================================
	struct VERTEX { 
		FLOAT X, Y, Z; 
		FLOAT U, V; 
	};
	//==================================================================

public:
	Engine(ID3D11Device * device, ID3D11DeviceContext * devcon, IDXGISwapChain * swapchain, ID3D11RenderTargetView * backbuffer, ID3D11DepthStencilView * depthbuffer);
	~Engine();

	bool InitPipeline();

	void MoveCamera(double deltaSec);

	void RenderFrame(double deltaSec);

	void RenderMesh(Mesh * mesh);

	void LoadProjection(glmath::mat4 * projection);
	void LoadViewMatrix(glmath::mat4 * viewMatrix);
	/*BAD */
	void LoadUniform(int offset, void * data, int size);
	void LoadUniforms(UNIFORMS * uniforms);

	void Cleanup();

};

