#include "Loader.h"



Loader::Loader(ID3D11Device * device){

	this->device = device;

}


Loader::~Loader(){

}

Mesh * Loader::LoadMesh(UINT * indices, int indexBufferSize, void * vertices, int vertexSize, int numVertices, bool dynamic){

	ID3D11Buffer * pVertex, * pIndex;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bd.ByteWidth = indexBufferSize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : NULL;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = indices;
	
	HRESULT hr = device->CreateBuffer(&bd, &initData, &pIndex);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create index buffer", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	ZeroMemory(&bd, sizeof(bd));
	ZeroMemory(&initData, sizeof(initData));

	bd.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bd.ByteWidth = vertexSize * numVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : NULL;

	initData.pSysMem = vertices;

	hr = device->CreateBuffer(&bd, &initData, &pVertex);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create vertex buffer", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	return new Mesh(pVertex, pIndex, numVertices, indexBufferSize / sizeof(UINT));
}

Texture * Loader::LoadTexture(LPCSTR fileName) {
	
	ID3D11ShaderResourceView * pTexture;
	HRESULT hr;
	D3DX11CreateShaderResourceViewFromFile(device, fileName, NULL, NULL, &pTexture, &hr);
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to load texture", "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
		return nullptr;
	}

	return new Texture(pTexture);

}
