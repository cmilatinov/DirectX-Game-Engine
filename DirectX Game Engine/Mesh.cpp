#include "Mesh.h"



Mesh::Mesh(ID3D11Buffer * pVertBuffer, ID3D11Buffer * pIndBuffer, int numVert, int numInd){
	
	this->pVertBuffer = pVertBuffer;
	this->pIndBuffer = pIndBuffer;

	this->numVert = numVert;
	this->numInd = numInd;

}

Mesh::~Mesh(){

	pVertBuffer->Release();
	pIndBuffer->Release();

}


int Mesh::GetVertexCount(){
	return numVert;
}

int Mesh::GetIndexCount(){
	return numInd;
}

ID3D11Buffer *const * Mesh::GetVertexBuffer(){
	return &pVertBuffer;
}

ID3D11Buffer * Mesh::GetIndexBuffer(){
	return pIndBuffer;
}

bool Mesh::DrawIndexed(){
	return !(pIndBuffer == NULL) || !(pIndBuffer == nullptr);
}
