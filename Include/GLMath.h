#pragma once

#include <cmath>

#define PI 3.14159265358979

namespace glmath {

	//===================== Namespace Functions =============================

	double rad(double degrees);
	double deg(double radians);

	float fovy(float fovx, float aspect = 16.0f / 9.0f);

}