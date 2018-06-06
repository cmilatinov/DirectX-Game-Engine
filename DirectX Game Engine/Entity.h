#include <GLMatrix.h>

#include "Mesh.h"
#pragma once
class Entity
{
private:
	Mesh * mesh;
	glmath::vec3 pos, rot;
	float scale;

public:
	Entity(Mesh * mesh);
	~Entity();

	void SetPosition(glmath::vec3 pos);
	void SetRotation(glmath::vec3 rot);
	void SetScale(float scale);

	glmath::mat4 GetModelMatrix();
};

