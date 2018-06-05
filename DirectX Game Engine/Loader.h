#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx11.h>

#include "Mesh.h"
#include "Texture.h"

#pragma once
class Loader
{
protected:
	ID3D11Device * device;

public:
	Loader(ID3D11Device * device);
	~Loader();

	Mesh * LoadMesh(UINT * indices, int indexBufferSize, void * vertices, int vertexSize, int numVertices, bool dynamic);

	Texture * LoadTexture(LPCSTR fileName);

};

