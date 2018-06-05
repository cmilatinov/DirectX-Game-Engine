#include "ShaderProgram.h"
#include <string>


ShaderProgram::ShaderProgram(ID3D11Device * device, ID3D11DeviceContext * devcon, LPCSTR vertexShader, LPCSTR pixelShader){
	
	// load and compile the two shaders
	D3DX11CompileFromFile(vertexShader, 0, 0, "main", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile(pixelShader, 0, 0, "main", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	// encapsulate both shaders into shader objects
	device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

}


ShaderProgram::~ShaderProgram(){
	
	// cleanup
	pLayout->Release();
	pVS->Release();
	pPS->Release();

}

void ShaderProgram::Use(ID3D11DeviceContext * devcon){
	
	// set the active shader objects
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	// set the active input layout 
	devcon->IASetInputLayout(pLayout);

}
