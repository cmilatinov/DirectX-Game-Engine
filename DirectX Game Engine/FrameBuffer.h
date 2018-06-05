#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>

#pragma once
class FrameBuffer
{
private:
	ID3D11Texture2D *				pTexture;
	ID3D11RenderTargetView *		pTargetView;
	ID3D11ShaderResourceView *		pShaderResource;

	void InitBuffer(ID3D11Device * device, int texWidth, int texHeight);

public:
	FrameBuffer(ID3D11Device * device, int texWidth, int texHeight);
	~FrameBuffer();

	ID3D11RenderTargetView * GetRenderTargetView();
	ID3D11ShaderResourceView * GetShaderResourceView();
};

