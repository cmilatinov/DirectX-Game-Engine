#pragma once

#include "GLVector.h"

#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

namespace glmath {
	
	//========================== Data Types =================================

	union mat4  {
		mat4();
		mat4(float m00, float m01, float m02, float m03,
			 float m10, float m11, float m12, float m13,
			 float m20, float m21, float m22, float m23,
			 float m30, float m31, float m32, float m33);

		float m[16];
		struct {
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};

		mat4 translate(vec3 vec);
		mat4 translate(vec3 vec, mat4 * dest);

		mat4 scale(vec3 vec);
		mat4 scale(vec3 vec, mat4 * dest);

		mat4 rotate(float angle, vec3 vec);
		mat4 rotate(float angle, vec3 vec, mat4 * dest);

		mat4 transpose();
		mat4 transpose(mat4 * dest);

		mat4 inverse();
		mat4 inverse(mat4 * dest);
		
		float determinant();

		friend mat4 operator+(const mat4& left, const mat4& right);
		friend mat4 operator-(const mat4& left, const mat4& right);
		friend mat4 operator*(const mat4& left, const mat4& right);

		friend vec4 operator*(const mat4& left, const vec4& right);
		
	};

	//========================== Constants ==================================

	const mat4 Mat4Identity = { 1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								0, 0, 0, 1 };

	//==================== Namespace Functions ==============================

	mat4 translate(vec3 vec, mat4 * src, mat4 * dest);
	mat4 scale(vec3 vec, mat4 * src, mat4 * dest);
	mat4 rotate(float angle, vec3 axis, mat4 * src, mat4 * dest);

	mat4 transpose(mat4 * src, mat4 * dest);

	mat4 perspective(float width, float height, float fov, bool rightHanded);
	mat4 perspective(float width, float height, float fov, float near_plane, float far_plane, bool rightHanded);

	mat4 inverse(mat4 * src, mat4 * dest);

	float determinant3x3(float t00, float t01, float t02,
						 float t10, float t11, float t12,
						 float t20, float t21, float t22);

	vec4 transform(mat4 left, vec4 right, vec4 * dest);
}