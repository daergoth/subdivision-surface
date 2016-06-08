#pragma once
#include <algorithm>
#include "Matrix4.h"
#include "Subdivision.h"

class Camera {
public:
	constexpr static GLfloat PI = 3.141592654;

	static GLboolean wireframeEnabled;

	Camera(Vector4 eye_, Vector4 up_, Vector4 light_, GLfloat dist, GLfloat s);
	~Camera();
	void draw();
	void move(GLfloat angle_var, GLfloat height_var, GLfloat deep_var);
private:
	Matrix4 sceneMatrix_A, sceneMatrix_B;

	GLfloat distance, s, angle = PI/2;
	Vector4 eye, up, light;
	void createSceneMatrix();
};