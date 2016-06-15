#include "ObjLoader.h"

std::vector<Vertex> ObjLoader::vertices = std::vector<Vertex>();
std::vector<HalfEdge> ObjLoader::halfEdges = std::vector<HalfEdge>();
std::vector<Face> ObjLoader::faces = std::vector<Face>();

bool ObjLoader::loadFile(std::string filename)
{
	vertices.clear();
	halfEdges.clear();
	faces.clear();

	std::ifstream file(filename);

	GLfloat sum_x = 0;

	if (!file) {
		std::cerr << "Invalid file name!" << std::endl;
		return false;
	}

	for (std::string line; std::getline(file, line); )
	{
		if (line[0] == 'v' && line[1] == ' ' ) {
			GLfloat x, y, z;

			sscanf_s(line.c_str(), "v %f %f %f", &x, &y, &z);

			sum_x += std::fabs(x);

			vertices.push_back({ new Vector4(x, z, -y), -1 });
		}
		else if (line[0] == 'f' && line[1] == ' ') {

			GLint a, b, c;
			
			if (line.find("//") != std::string::npos) {
				sscanf_s(line.c_str(), "f %d//%*d %d//%*d %d//%*d", &a, &b, &c);
			}
			else if (line.find("/") != std::string::npos) {
				sscanf_s(line.c_str(), "f %d/%*d %d/%*d %d/%*d", &a, &b, &c);
			}
			else {
				sscanf_s(line.c_str(), "f %d %d %d", &a, &b, &c);
			}

			GLint vert_size = vertices.size();

			faces.push_back({ { a < 0 ? vert_size-a : a-1, b < 0 ? vert_size - b : b - 1, c < 0 ? vert_size - c : c - 1 } });

		}
	}

	sum_x /= vertices.size();

	for (int i = 0; i < vertices.size(); ++i)
	{
		*vertices[i].vector /= sum_x;
	}

	std::cerr << "Loaded " << vertices.size() << " vertices." << std::endl;

	std::cerr << "Loaded " << faces.size() << " faces." << std::endl;

	return generateHalfEdge();
}

bool ObjLoader::generateHalfEdge()
{
	std::vector<std::vector<GLint> > edgeMap(vertices.size(), std::vector<GLint>());

	for (GLint he = 0; he < 3 * faces.size(); ++he) {
		//std::cout << "Current HalfEdge num: " << he << std::endl;

		GLint face = he / 3;

		// vertex update
		vertices[faces[face].verts[(he + 1) % 3]].halfEdge = he;

		// halfedge constr.
		GLint pair = -1;

		for (std::vector<GLint>::iterator it = edgeMap[faces[face].verts[(he + 1) % 3]].begin(); it != edgeMap[faces[face].verts[(he + 1) % 3]].end(); ++it) {
			if (faces[face].verts[he % 3] == faces[*it/3].verts[(*it + 1) % 3]) {
				pair = *it;
				halfEdges[*it].pair = he;
				edgeMap[faces[face].verts[(he + 1) % 3]].erase(it);
				break;
			}
		}
		
		if (pair == -1) {
			edgeMap[faces[face].verts[he % 3]].push_back(he);
		}

		halfEdges.push_back({ pair });

	}

	std::cerr << "Loading complete!" << std::endl;
	return true;
}
