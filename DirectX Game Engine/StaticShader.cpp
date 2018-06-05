#include "StaticShader.h"



StaticShader::StaticShader(ID3D11Device * device, ID3D11DeviceContext * devcon, LPCSTR vertexShader, LPCSTR pixelShader) : ShaderProgram(device, devcon, vertexShader, pixelShader){
	
	// create input layout
	CreateInputLayout(device);

}


StaticShader::~StaticShader(){
	
}

void StaticShader::CreateInputLayout(ID3D11Device * device){
	
	// create the input layout struct
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// create a pointer to an input layout object
	device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	
}

void StaticShader::Use(ID3D11DeviceContext * devcon) {

	ShaderProgram::Use(devcon);

}
