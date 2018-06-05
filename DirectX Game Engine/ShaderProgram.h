#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>

#pragma once
class ShaderProgram
{
protected:
	ID3D11VertexShader * pVS;               // the pointer to the vertex shader
	ID3D11PixelShader * pPS;                // the pointer to the pixel shader
	ID3D11InputLayout * pLayout;            // the pointer to the input layout
	ID3D10Blob * VS, * PS;					// the pointers to the blob objects

public:
	ShaderProgram(ID3D11Device * device, ID3D11DeviceContext * devcon, LPCSTR vertexShader, LPCSTR pixelShader);
	~ShaderProgram();

	virtual void CreateInputLayout(ID3D11Device * device) = 0;

	virtual void Use(ID3D11DeviceContext * devcon);

};

