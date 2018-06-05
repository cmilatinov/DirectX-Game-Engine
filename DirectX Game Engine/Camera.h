#include <GLVector.h>
#include <GLMatrix.h>

#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>

#pragma once
class Camera
{

private:
	glmath::vec3 pos;
	glmath::vec3 rot;

public:
	Camera();
	~Camera();

	glmath::vec3 GetForwardVector();
	glmath::vec3 GetRightVector();

	glmath::mat4 CreateViewMatrix();
	glmath::mat4 CreateRotationMatrix();

	glmath::vec3 GetPosition();
	glmath::vec3 GetRotation();

	float GetYaw();
	float GetPitch();
	float GetRoll();

	void SetPosition(glmath::vec3 position);
	void SetRotation(glmath::vec3 rotation);
	
	void SetYaw(float yaw);
	void SetPitch(float pitch);
	void SetRoll(float roll);
		
};

