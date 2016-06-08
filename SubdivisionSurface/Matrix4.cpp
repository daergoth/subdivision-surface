#include "Matrix4.h"

Matrix4::Matrix4()
{
	values = reserveMemory();

	setIdentity();
}

Matrix4::Matrix4(const Matrix4 & other)
{
	values = reserveMemory();

	for (GLint i = 0; i < 4; ++i) {
		for (GLint j = 0; j < 4; ++j) {
			values[i][j] = other.values[i][j];
			//std::cout << values[i][j] << " = " << other.values[i][j] << std::endl;
		}
	}
}

Matrix4::Matrix4(GLfloat** _values)
{
	values = _values;
}

Matrix4::Matrix4(const std::initializer_list<std::initializer_list<GLfloat>>& init)
{
	values = reserveMemory();

	GLint i = 0, j = 0;

	for (auto list_it = init.begin(); list_it != init.end(); ++list_it) {
		for (auto val_it = list_it->begin(); val_it != list_it->end(); ++val_it) {
			//std::cout << "*val_it = " << *val_it << std::endl;

			values[i][j] = *val_it;

			//std::cout << "values[i][j] = " << values[i][j] << std::endl;

			j++;
		}
		j = 0;
		i++;
	}
}

Matrix4::~Matrix4()
{
	freeMemory();
}

Matrix4 Matrix4::operator*(const Matrix4 & other)
{
	GLfloat** tempVal = reserveMemory();

	for (GLint i = 0; i < 4; ++i)
	{
		for (GLint j = 0; j < 4; ++j)
		{
			GLfloat sum = 0;

			for (GLint k = 0; k < 4; ++k)
			{
				sum += values[i][k] * other.values[k][j];
			}

			tempVal[i][j] = sum;
		}
	}

	return Matrix4(tempVal);
}

Vector4 Matrix4::operator*(const Vector4 & v)
{
	GLfloat tx = (v.x * values[0][0]) + (v.y * values[0][1]) + (v.z * values[0][2]) + (v.h * values[0][3]);
	GLfloat ty = (v.x * values[1][0]) + (v.y * values[1][1]) + (v.z * values[1][2]) + (v.h * values[1][3]);
	GLfloat tz = (v.x * values[2][0]) + (v.y * values[2][1]) + (v.z * values[2][2]) + (v.h * values[2][3]);
	GLfloat th = (v.x * values[3][0]) + (v.y * values[3][1]) + (v.z * values[3][2]) + (v.h * values[3][3]);

	return Vector4(tx, ty, tz, th);
}

void Matrix4::destructMultiply(const Matrix4 & other)
{
	GLfloat** tempVal = reserveMemory();

	for (GLint i = 0; i < 4; ++i)
	{
		for (GLint j = 0; j < 4; ++j)
		{
			GLfloat sum = 0;

			for (GLint k = 0; k < 4; ++k)
			{
				sum += values[i][k] * other.values[k][j];
			}

			tempVal[i][j] = sum;
		}
	}

	freeMemory();
	values = tempVal;

}

Matrix4 & Matrix4::operator=(const Matrix4 & other)
{
	for (GLint i = 0; i < 4; ++i) {
		for (GLint j = 0; j < 4; ++j) {
			values[i][j] = other.values[i][j];
			//std::cout << values[i][j] << " = " << other.values[i][j] << std::endl;
		}
	}

	return *this;
}

void Matrix4::setIdentity()
{
	for (GLint i = 0; i < 4; ++i)
	{
		for (GLint j = 0; j < 4; ++j)
		{
			if (i != j)
			{
				values[i][j] = 0;
			}
			else
			{
				values[i][j] = 1;
			}
		}
	}
	return;
}

void Matrix4::setZero()
{
	for (GLint i = 0; i < 4; ++i)
	{
		for (GLint j = 0; j < 4; ++j)
		{
			values[i][j] = 0;
		}
	}
}

GLfloat& Matrix4::at(GLint x, GLint y)
{
	return values[x][y];
}

const GLfloat& Matrix4::at(GLint x, GLint y) const
{
	return values[x][y];
}

GLfloat** Matrix4::reserveMemory()
{
	GLfloat** v = new GLfloat*[4];

	v[0] = new GLfloat[4];
	v[1] = new GLfloat[4];
	v[2] = new GLfloat[4];
	v[3] = new GLfloat[4];

	return v;
}

void Matrix4::freeMemory()
{
	delete[] values[0];
	delete[] values[1];
	delete[] values[2];
	delete[] values[3];

	delete[] values;
}

