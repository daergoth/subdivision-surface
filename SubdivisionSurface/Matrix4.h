#pragma once
#include <initializer_list>
#include <memory>
#include "Vector4.h"

class Matrix4 {
public:
	GLfloat** values;

	Matrix4();

	Matrix4(const Matrix4& other);

	Matrix4(GLfloat** values);

	Matrix4(const std::initializer_list<std::initializer_list<GLfloat> >&);

	~Matrix4();

	GLfloat& at(GLint x, GLint y);
	const GLfloat& at(GLint x, GLint y) const;

	Matrix4 operator*(const Matrix4 & other);
	Vector4 operator*(const Vector4 &);
	void destructMultiply(const Matrix4 & other);

	Matrix4& operator=(const Matrix4& other);

	void setIdentity();
	void setZero();

	friend std::ostream& operator<<(std::ostream& os, const Matrix4& obj)
	{
		for (GLint i = 0; i < 4; ++i) {
			for (GLint j = 0; j < 4; ++j) {
				os << obj.at(i, j) << ", ";
			}
			os << std::endl;
		}

		return os;
	}

private:
	static GLfloat** reserveMemory();

	void freeMemory();

};