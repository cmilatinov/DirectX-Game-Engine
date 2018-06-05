#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>

#pragma once
class Texture
{
private:
	ID3D11ShaderResourceView * pTexture;

public:
	Texture(ID3D11ShaderResourceView * pTexture);
	~Texture();

	ID3D11ShaderResourceView *const * GetResource();

};

