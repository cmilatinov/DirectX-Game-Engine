#include "Engine.h"

float angle = 0.0f;
float angle2 = 90.0f;

float translate = 0.0f;

Engine::Engine(ID3D11Device * device, ID3D11DeviceContext * devcon, IDXGISwapChain * swapchain, ID3D11RenderTargetView * backbuffer, ID3D11DepthStencilView * depthbuffer){
	
	this->device = device;
	this->devcon = devcon;
	this->swapchain = swapchain;

	this->backbuffer = backbuffer;
	this->depthbuffer = depthbuffer;

	this->loader = new Loader(device);

}

Engine::~Engine(){

	delete loader;
	delete shader;
	delete[] buffers;

}

bool Engine::InitPipeline(){

	// create and set constant buffers
	D3D11_BUFFER_DESC bd;

	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(glmath::mat4);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = device->CreateBuffer(&bd, NULL, &pProjMatBuffer);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create constant buffer", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}
	
	hr = device->CreateBuffer(&bd, NULL, &pViewMatBuffer);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create constant buffer", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(UNIFORMS);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = device->CreateBuffer(&bd, NULL, &pConstBuffer);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create constant buffer", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	buffers = new ID3D11Buffer*[3]{pProjMatBuffer, pViewMatBuffer, pConstBuffer};

	// set the active constant buffers
	// no need to call this anymore
	devcon->VSSetConstantBuffers(0, 3, buffers);

	// create and set the sampler state
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
	hr = device->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create sampler state", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}
	devcon->PSSetSamplers(0, 1, &samplerState);

	// create and set rasterizer state
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(rastDesc));
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.MultisampleEnable = true;
	rastDesc.FrontCounterClockwise = true;

	hr = device->CreateRasterizerState(&rastDesc, &rasterizerState);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create rasterizer state", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}
	devcon->RSSetState(rasterizerState);

	// create the shader program
	shader = new StaticShader(device, devcon, "staticVertexShader.hlsl", "staticPixelShader.hlsl");

	// load projection matrix
	glmath::mat4 projectionMatrix = glmath::perspective(1920, 1080, 70, false);
	LoadProjection(&projectionMatrix);

	// load view matrix
	glmath::mat4 viewMatrix = glmath::Mat4Identity;
	LoadViewMatrix(&viewMatrix);

	// load uniforms
	uniforms.modelMatrix = glmath::Mat4Identity;
	uniforms.modelMatrix.translate(glmath::vec3(0, 0, 5));
	uniforms.modelMatrix.rotate(45.0f, glmath::vec3(0, 1, 0));
	uniforms.modelMatrix.rotate(45.0f, glmath::vec3(1, 0, 0));
	LoadUniforms(&uniforms);

	// initialize graphics
	
	// create vertices
	VERTEX vertices[] =
	{
		{ -0.5f, 0.5f, 0.5f, 0.0f, 0.0f },
		{ -0.5f, -0.5f, 0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.5f, 1.0f, 1.0f },
		{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f },

		{ -0.5f, 0.5f, -0.5f, 0.0f, 0.0f },
		{ -0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, -0.5f, 1.0f, 1.0f },
		{ 0.5f, 0.5f, -0.5f, 1.0f, 0.0f },
		
		{ 0.5f, 0.5f, 0.5f, 0.0f, 0.0f },
		{ 0.5f, -0.5f, 0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, -0.5f, 1.0f, 1.0f },
		{ 0.5f, 0.5f, -0.5f, 1.0f, 0.0f },

		{ -0.5f, 0.5f, 0.5f, 0.0f, 0.0f },
		{ -0.5f, -0.5f, 0.5f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, -0.5f, 1.0f, 1.0f },
		{ -0.5f, 0.5f, -0.5f, 1.0f, 0.0f },

		{ -0.5f, 0.5f, -0.5f, 0.0f, 0.0f },
		{ -0.5f, 0.5f, 0.5f, 0.0f, 1.0f },
		{ 0.5f, 0.5f, 0.5f, 1.0f, 1.0f },
		{ 0.5f, 0.5f, -0.5f, 1.0f, 0.0f },

		{ -0.5f, -0.5f, -0.5f, 0.0f, 0.0f },
		{ -0.5f, -0.5f, 0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.5f, 1.0f, 1.0f },
		{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f }
	};

	// create indices
	unsigned int indices[] =  { 0,3,1,
								3,2,1,
								4,5,7,
								7,5,6,
								8,11,9,
								11,10,9,
								12,13,15,
								15,13,14,
								16,19,17,
								19,18,17,
								20,21,23,
								23,21,22 };

	mesh = loader->LoadMesh(indices, sizeof(indices), vertices, sizeof(VERTEX), 24, false);

	texture = loader->LoadTexture("dab.png");

	camera = new Camera();

	return true;
}

void Engine::MoveCamera(double deltaSec){

	

}

void Engine::RenderFrame(double deltaSec){

	// clear the back buffer to a the specified color
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	devcon->ClearDepthStencilView(depthbuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
		// do 3D rendering on the back buffer here
		uniforms.modelMatrix = glmath::Mat4Identity;
		uniforms.modelMatrix.translate(glmath::vec3(0, 0, 5));
		uniforms.modelMatrix.rotate(angle, glmath::vec3(0, 1, 0));
		uniforms.modelMatrix.rotate(angle2,  glmath::vec3(1, 0, 0));
		LoadUniforms(&uniforms);
		angle += 360.0f * deltaSec;
		angle2 += 360.0f * deltaSec;

		RenderMesh(mesh);

		uniforms.modelMatrix = glmath::Mat4Identity;
		uniforms.modelMatrix.translate(glmath::vec3(0, 0, 10 + translate));
		LoadUniforms(&uniforms);
		translate -= deltaSec * 20.0f;

		RenderMesh(mesh);

	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);

}

void Engine::RenderMesh(Mesh * mesh) {
	
	// set the active shader	
	shader->Use(devcon);

	// select which vertex buffer to display
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer(), &stride, &offset);
	devcon->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	devcon->PSSetShaderResources(0, 1, texture->GetResource());

	// select which primtive type we are using
	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer
	devcon->DrawIndexed(mesh->GetIndexCount(), 0, 0);

}

void Engine::LoadProjection(glmath::mat4 * projection){
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pProjMatBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, projection, sizeof(glmath::mat4));
	devcon->Unmap(pProjMatBuffer, NULL);
}

void Engine::LoadViewMatrix(glmath::mat4 * viewMatrix){
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pViewMatBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, viewMatrix, sizeof(glmath::mat4));
	devcon->Unmap(pViewMatBuffer, NULL);
}

void Engine::LoadUniform(int offset, void * data, int size){
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pConstBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy((void*) (reinterpret_cast<BYTE*>(ms.pData) + offset), data, size);
	for(int c = 0; c < size; c++)
		std::cout << std::hex << (reinterpret_cast<BYTE*>(ms.pData) + c);
	devcon->Unmap(pConstBuffer, NULL);
}

void Engine::LoadUniforms(UNIFORMS * uniforms){
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pConstBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, uniforms, sizeof(UNIFORMS));
	devcon->Unmap(pConstBuffer, NULL);
}

void Engine::Cleanup(){
	pProjMatBuffer->Release();
	pViewMatBuffer->Release();
	pConstBuffer->Release();

	delete shader;
}
