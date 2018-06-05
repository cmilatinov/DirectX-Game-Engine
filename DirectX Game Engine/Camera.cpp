#include "Camera.h"


Camera::Camera(){
}

Camera::~Camera(){
}

glmath::vec3 Camera::GetForwardVector(){
	glmath::mat4 transform = glmath::Mat4Identity;
	transform.rotate(rot.x, glmath::vec3(1, 0, 0));
	transform.rotate(rot.y, glmath::vec3(0, 1, 0));
	transform.rotate(rot.z, glmath::vec3(0, 0, 1));
	glmath::vec4 resultVec4 = glmath::vec4(0, 0, 1, 1);
	resultVec4 = transform * resultVec4;
	return (1.0f / resultVec4.w) * glmath::vec3(resultVec4.x, resultVec4.y, resultVec4.z);
}

glmath::vec3 Camera::GetRightVector(){
	glmath::vec3 forward = GetForwardVector();
	glmath::mat4 transform = glmath::Mat4Identity;
	transform.rotate(90.0f, glmath::vec3(0, 1, 0));
	glmath::vec4 resultVec4 = glmath::vec4(forward.x, forward.y, forward.z, 1);
	resultVec4 = transform * resultVec4;
	return (1.0f / resultVec4.w) * glmath::vec3(resultVec4.x, resultVec4.y, resultVec4.z);
}

glmath::mat4 Camera::CreateViewMatrix(){
	glmath::mat4 transform = glmath::Mat4Identity;
	transform.translate(-1.0f * pos);
	transform.rotate(rot.x, glmath::vec3(1, 0, 0));
	transform.rotate(rot.y, glmath::vec3(0, 1, 0));
	transform.rotate(rot.z, glmath::vec3(0, 0, 1));
	return transform;
}

glmath::mat4 Camera::CreateRotationMatrix(){
	glmath::mat4 transform = glmath::Mat4Identity;
	transform.rotate(rot.x, glmath::vec3(1, 0, 0));
	transform.rotate(rot.y, glmath::vec3(0, 1, 0));
	transform.rotate(rot.z, glmath::vec3(0, 0, 1));
	return transform;
}

glmath::vec3 Camera::GetPosition(){
	return pos;
}

glmath::vec3 Camera::GetRotation(){
	return rot;
}

float Camera::GetPitch() {
	return rot.x;
}

float Camera::GetYaw(){
	return rot.y;
}

float Camera::GetRoll(){
	return rot.z;
}

void Camera::SetPosition(glmath::vec3 position) {
	this->pos = position;
}


void Camera::SetRotation(glmath::vec3 rotation){
	this->rot = rotation;
}

void Camera::SetPitch(float pitch) {
	rot.x = pitch;
}

void Camera::SetYaw(float yaw){
	rot.y = yaw;
}

void Camera::SetRoll(float roll){
	rot.z = roll;
}
