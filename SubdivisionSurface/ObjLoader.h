#pragma once
#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include "HalfEdge.h"

class ObjLoader {
public:
	static bool loadFile(std::string);

	static std::vector<Vertex> getVertices() {
		return vertices;
	}

	static std::vector<HalfEdge> getHalfEdges() {
		return halfEdges;
	}

	static std::vector<Face> getFaces() {
		return faces;
	}

private:
	static std::vector<Vertex> vertices;
	static std::vector<HalfEdge> halfEdges;
	static std::vector<Face> faces;

	static bool generateHalfEdge();
};