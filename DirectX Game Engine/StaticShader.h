#include "ShaderProgram.h"
#include "GLMatrix.h"

#pragma once
class StaticShader :
	public ShaderProgram {

private:
	void CreateInputLayout(ID3D11Device * device);

public:
	StaticShader(ID3D11Device * device, ID3D11DeviceContext * devcon, LPCSTR vertexShader, LPCSTR pixelShader);
	~StaticShader();
		
	void Use(ID3D11DeviceContext * devcon);
};

