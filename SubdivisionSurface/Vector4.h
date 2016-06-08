#pragma once
#include <iostream>
#include "GLFW\glfw3.h"

class Vector4 {
public:
	GLfloat x, y, z, h;

	Vector4(GLfloat x = 0.0, GLfloat y = 0.0, GLfloat z = 0.0, GLfloat h = 1.0);

	Vector4 operator+(const Vector4 &);
	Vector4 operator-(const Vector4 &);
	Vector4 operator*(const GLfloat &);
	Vector4 operator/(const GLfloat &);

	Vector4 operator-() const;

	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(GLfloat r);
	Vector4& operator/=(GLfloat r);
	
	Vector4& normalize();

	static GLfloat len(const Vector4 &);
	static GLfloat len2(const Vector4 &);
	static GLfloat innerProduct(const Vector4 &, const Vector4 &);
	static Vector4 crossProduct(const Vector4 &, const Vector4 &);

	friend std::ostream& operator<<(std::ostream& os, const Vector4& obj)
	{
		os << "[" << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.h << "]";
		return os;
	}
};