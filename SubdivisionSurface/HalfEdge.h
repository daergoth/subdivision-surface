#pragma once
#include "Vector4.h"

struct Vertex {
	Vector4* vector;

	GLint halfEdge;
};

struct Face {
	GLint verts[3];
};

struct HalfEdge {
	GLint pair;
};

struct Normal {
	Vector4* vector;
};