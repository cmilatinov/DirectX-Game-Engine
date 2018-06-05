#include "Texture.h"



Texture::Texture(ID3D11ShaderResourceView * pTexture){

	this->pTexture = pTexture;

}


Texture::~Texture(){

	pTexture->Release();

}

ID3D11ShaderResourceView *const * Texture::GetResource(){
	return &pTexture;
}
