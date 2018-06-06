#pragma once

#include "GLMath.h"

namespace glmath {

	//========================== Data Types =================================

	union vec2 {
		vec2();
		vec2(float value);
		vec2(float x, float y);
	
		struct { float x, y; };
		struct { float s, t; };
		float v[2];

		double length() const;
		
		vec2 normalize();
		vec2 normalize(vec2 * dest);

		vec2 friend operator+(const vec2& left, const vec2& right);
		vec2 friend operator-(const vec2& left, const vec2& right);

		vec2 operator+=(const vec2& right);
		vec2 operator-=(const vec2& right);

		vec2 friend operator*(const double& left, const vec2& right);
		vec2 friend operator*(const vec2& left, const double& right);

		vec2 friend operator/(const vec2& left, const double& right);
	};

	union vec3 {
		vec3();
		vec3(float value);
		vec3(float x, float y, float z);
		
		struct { float x, y, z; };
		struct { float r, g, b; };
		float v[3];
	
		double length() const;

		vec3 normalize();
		vec3 normalize(vec3 * dest);

		vec3 friend operator+(const vec3& left, const vec3& right);
		vec3 friend operator-(const vec3& left, const vec3& right);
		
		vec3 operator+=(const vec3& right);
		vec3 operator-=(const vec3& right);

		vec3 friend operator*(const double& left, const vec3& right);
		vec3 friend operator*(const vec3& left, const double& right);

		vec3 friend operator/(const vec3& left, const double& right);
	};

	union vec4 {
		vec4();
		vec4(float value);
		vec4(float x, float y, float z, float w);

		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
		float v[4];

		double length() const;

		vec4 normalize();
		vec4 normalize(vec4 * dest);

		vec4 friend operator+(const vec4& left, const vec4& right);
		vec4 friend operator-(const vec4& left, const vec4& right);

		vec4 operator+=(const vec4& right);
		vec4 operator-=(const vec4& right);

		vec4 friend operator*(const double& left, const vec4& right);
		vec4 friend operator*(const vec4& left, const double& right);

		vec4 friend operator/(const vec4& left, const double& right);
	};

	//===================== Namespace Functions =============================
	
	vec2 normalize(vec2 * src, vec2 * dest);
	vec3 normalize(vec3 * src, vec3 * dest);
	vec4 normalize(vec4 * src, vec4 * dest);

	vec3 cross(const vec2& left, const vec2& right);
	vec3 cross(const vec3& left, const vec3& right);
	
	double dot(const vec2& left, const vec2& right);
	double dot(const vec3& left, const vec3& right);
	double dot(const vec4& left, const vec4& right);

	double angle(const vec2& left, const vec2& right);
	double angle(const vec3& left, const vec3& right);

}