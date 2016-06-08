#pragma once
#include <iostream>
#include <vector>
#include "Camera.h"
#include "Matrix4.h"
#include "HalfEdge.h"

class Subdivision {
public:
	constexpr static GLint SCREEN_WIDTH = 1280;
	constexpr static GLint SCREEN_HEIGHT = 720;
	constexpr static GLfloat DELTA_ANGLE = 1;
	constexpr static GLfloat DELTA_HEIGHT = .25;
	constexpr static GLfloat DELTA_DEEP = 1;

	static std::vector<Vertex> vertices;
	static std::vector<Face> faces;
	static std::vector<HalfEdge> halfEdges;
	static std::vector<Normal> normals;

	static void createObject();
	static void subdivision();

private:
	static std::vector<Vertex> new_vertices;
	static std::vector<Face> new_faces;
	static std::vector<HalfEdge> new_halfEdges;
	static std::vector<GLint> edgeMap;

	static void newEdgeVerts();
	static void updateOrigVerts();
	static void splitFaces();
	static void createHalfEdgeInfo();
	static GLint nIndex(GLint ei, GLint type);
};