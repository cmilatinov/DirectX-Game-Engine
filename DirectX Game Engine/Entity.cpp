#include "Entity.h"



Entity::Entity(Mesh * mesh){

	this->mesh = mesh;

}


Entity::~Entity(){



}

void Entity::SetPosition(glmath::vec3 pos){
	this->pos = pos;
}

void Entity::SetRotation(glmath::vec3 rot){
	this->rot = rot;
}

void Entity::SetScale(float scale){
	this->scale = scale;
}

glmath::mat4 Entity::GetModelMatrix(){
	glmath::mat4 result = glmath::Mat4Identity;

	result.translate(pos);
	result.rotate(rot.x, glmath::vec3(1, 0, 0));
	result.rotate(rot.y, glmath::vec3(0, 1, 0));
	result.rotate(rot.z, glmath::vec3(0, 0, 1));
	result.scale(glmath::vec3(scale));

	return result;
}
