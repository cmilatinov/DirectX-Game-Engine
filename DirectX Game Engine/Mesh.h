#include <d3d11.h>
#include <d3dx11.h>

#pragma once
class Mesh
{
protected:
	ID3D11Buffer * pVertBuffer;
	ID3D11Buffer * pIndBuffer;

	int numVert, numInd;

public:
	Mesh(ID3D11Buffer * pVertBuffer, ID3D11Buffer * pIndBuffer, int numVert, int numInd);
	~Mesh();

	int GetVertexCount();
	int GetIndexCount();

	ID3D11Buffer *const * GetVertexBuffer();
	ID3D11Buffer * GetIndexBuffer();

	bool DrawIndexed();

};

