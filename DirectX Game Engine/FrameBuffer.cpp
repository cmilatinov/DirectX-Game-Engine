#include "FrameBuffer.h"



FrameBuffer::FrameBuffer(ID3D11Device * device, int texWidth, int texHeight) {

	InitBuffer(device, texWidth, texHeight);

}


FrameBuffer::~FrameBuffer() {

	pTexture->Release();
	pTargetView->Release();
	pShaderResource->Release();

}

void FrameBuffer::InitBuffer(ID3D11Device * device, int texWidth, int texHeight) {

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = texWidth;
	texDesc.Height = texHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	HRESULT hr = device->CreateTexture2D(&texDesc, NULL, &pTexture);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create 2D texture", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}


	D3D11_RENDER_TARGET_VIEW_DESC renderDesc;
	ZeroMemory(&renderDesc, sizeof(renderDesc));
	renderDesc.Format = texDesc.Format;
	renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderDesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(pTexture, &renderDesc, &pTargetView);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create render target view", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
	ZeroMemory(&resourceDesc, sizeof(resourceDesc));
	resourceDesc.Format = texDesc.Format;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MostDetailedMip = 0;
	resourceDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(pTexture, &resourceDesc, &pShaderResource);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create render target view", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

}