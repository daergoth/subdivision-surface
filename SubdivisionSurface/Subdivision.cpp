#include "Subdivision.h"

std::vector<Vertex> Subdivision::vertices = std::vector<Vertex>();
std::vector<Face> Subdivision::faces = std::vector<Face>();
std::vector<HalfEdge> Subdivision::halfEdges = std::vector<HalfEdge>();
//std::vector<Normal> Subdivision::normals = std::vector<Normal>();

std::vector<Vertex> Subdivision::new_vertices = std::vector<Vertex>();
std::vector<Face> Subdivision::new_faces = std::vector<Face>();
std::vector<HalfEdge> Subdivision::new_halfEdges = std::vector<HalfEdge>();
std::vector<GLint> Subdivision::edgeMap = std::vector<GLint>();

Camera cam = Camera(Vector4(0,0,10), Vector4(0,1,0), Vector4(100, 100, 100), 10, 6.0);

void init () {
	glClearColor( 1.0, 1.0, 1.0, 0.0 );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f, Subdivision::SCREEN_WIDTH, 0.f, Subdivision::SCREEN_HEIGHT, 0.f, 1.f);
	
	Subdivision::createObject();
}

void Subdivision::createObject()
{
	vertices.push_back({ new Vector4(-1, -1, -1), 2});
	vertices.push_back({ new Vector4(1, -1, -1), 7});
	vertices.push_back({ new Vector4(1, -1, 1), 3 });
	vertices.push_back({ new Vector4(-1, -1, 1), 33 });

	vertices.push_back({ new Vector4(-1, 1, -1), 6 });
	vertices.push_back({ new Vector4(1, 1, -1), 15 });
	vertices.push_back({ new Vector4(1, 1, 1), 12 });
	vertices.push_back({ new Vector4(-1, 1, 1), 28 });

	faces.push_back({ { 0, 1, 2 } });
	faces.push_back({ { 0, 2, 3 } });

	faces.push_back({ { 0, 4, 1 } });
	faces.push_back({ { 1, 4, 5 } });

	faces.push_back({ { 1, 6, 2 } });
	faces.push_back({ { 1, 5, 6 } });

	faces.push_back({ { 5, 7, 6 } });
	faces.push_back({ { 4, 7, 5 } });

	faces.push_back({ { 2, 6, 3 } });
	faces.push_back({ { 3, 6, 7 } });

	faces.push_back({ { 3, 7, 4 } });
	faces.push_back({ { 0, 3, 4 } });

	// 0
	halfEdges.push_back({ 8 });
	halfEdges.push_back({ 14 });
	halfEdges.push_back({ 3 });
	// 1
	halfEdges.push_back({ 2 });
	halfEdges.push_back({ 26 });
	halfEdges.push_back({ 33});
	// 2
	halfEdges.push_back({ 35 });
	halfEdges.push_back({ 9 });
	halfEdges.push_back({ 0 });
	// 3
	halfEdges.push_back({ 7 });
	halfEdges.push_back({ 23 });
	halfEdges.push_back({ 15 });
	// 4
	halfEdges.push_back({ 17 });
	halfEdges.push_back({ 24 });
	halfEdges.push_back({ 1 });
	// 5
	halfEdges.push_back({ 11 });
	halfEdges.push_back({ 20});
	halfEdges.push_back({ 12 });
	// 6
	halfEdges.push_back({ 22 });
	halfEdges.push_back({ 28 });
	halfEdges.push_back({ 16 });
	// 7
	halfEdges.push_back({ 31 });
	halfEdges.push_back({ 18 });
	halfEdges.push_back({ 10 });
	// 8
	halfEdges.push_back({ 13 });
	halfEdges.push_back({ 27 });
	halfEdges.push_back({ 4 });
	// 9
	halfEdges.push_back({ 25 });
	halfEdges.push_back({ 19 });
	halfEdges.push_back({ 30 });
	// 10
	halfEdges.push_back({ 29 });
	halfEdges.push_back({ 21 });
	halfEdges.push_back({ 34 });
	// 11
	halfEdges.push_back({ 5 });
	halfEdges.push_back({ 32 });
	halfEdges.push_back({ 6 });
}

void Subdivision::subdivision()
{
	newEdgeVerts();

	updateOrigVerts();

	splitFaces();

	createHalfEdgeInfo();

	for (Vertex v : vertices) {
		delete v.vector;
	}

	vertices.clear();
	vertices = std::move(new_vertices);

	faces.clear();
	faces = std::move(new_faces);

	halfEdges.clear();
	halfEdges = std::move(new_halfEdges);
}

void Subdivision::newEdgeVerts()
{ 
	new_vertices.clear();

	GLint old_vert_num = vertices.size();
	GLint old_halfedge_num = halfEdges.size();

	new_vertices.reserve(old_vert_num + old_halfedge_num / 2);
	for (GLint i = 0; i < new_vertices.capacity(); ++i) {
		new_vertices.push_back({});
	}

	edgeMap.clear();
	edgeMap = std::vector<GLint>(old_halfedge_num, -1);

	GLint unused_slot = 0;
	for (GLint h = 0; h < old_halfedge_num; ++h) {
		if (edgeMap[h] == -1) {

			Vector4 vec = (*vertices[ faces[h / 3].verts[(h + 1)%3] ].vector + *vertices[ faces[halfEdges[h].pair / 3].verts[(halfEdges[h].pair + 1) % 3] ].vector) * 3.0/8.0
				+ (*vertices[faces[h / 3].verts[(h + 2) % 3]].vector + *vertices[faces[halfEdges[h].pair / 3].verts[(halfEdges[h].pair + 2) % 3]].vector) * 1.0 / 8.0;

			new_vertices[old_vert_num + unused_slot] = { new Vector4(vec), -1 };

			edgeMap[h] = old_vert_num + unused_slot;
			edgeMap[halfEdges[h].pair] = old_vert_num + unused_slot;

			unused_slot++;
		}
	}
}

void Subdivision::updateOrigVerts()
{
	for (GLint ov = 0; ov < vertices.size(); ++ov) {
		Vector4 sum;

		GLint n = 0;
		GLint start_edge = vertices[ov].halfEdge;
		GLint current_edge = start_edge;
		do {
			current_edge = halfEdges[current_edge].pair;

			sum += *(vertices[ faces[current_edge / 3].verts[(current_edge + 1) % 3] ].vector);
			n++;

			current_edge = 3 * (current_edge / 3) + ((current_edge + 2) % 3);
		} while (current_edge != start_edge);

		GLfloat b = (1.0 / 64.0) * (40 - std::pow(3 + 2 * std::cos((2 * Camera::PI)/n), 2));

		Vector4 nov = *vertices[ov].vector * (1 - b) + sum * (b / n);

		new_vertices[ov].vector = new Vector4(nov);
		new_vertices[ov].halfEdge = -1;
	}
}

void Subdivision::splitFaces()
{
	new_faces.clear();
	new_faces.reserve(4 * faces.size());

	for (GLint i = 0; i < new_faces.capacity(); ++i) {
		new_faces.push_back({});
	}

	for (GLint f = 0; f < faces.size(); ++f) {
		new_faces[4 * f] = { {edgeMap[f * 3 + 2], faces[f].verts[0], edgeMap[f * 3]} };

		new_faces[4 * f + 1] = { { edgeMap[f * 3], faces[f].verts[1], edgeMap[f * 3 + 1] } };

		new_faces[4 * f + 2] = { { edgeMap[f * 3 + 1], faces[f].verts[2], edgeMap[f * 3 + 2] } };

		new_faces[4 * f + 3] = { { edgeMap[f * 3 + 2], edgeMap[f * 3], edgeMap[f * 3 + 1] } };

		
		new_vertices[new_faces[4 * f].verts[1]].halfEdge = f * 12;
		new_vertices[new_faces[4 * f + 1].verts[1]].halfEdge = f * 12 + 3;
		new_vertices[new_faces[4 * f + 2].verts[1]].halfEdge = f * 12 + 6;
		
		new_vertices[edgeMap[f * 3]].halfEdge = f * 12 + 1;
		new_vertices[edgeMap[f * 3 + 1]].halfEdge = f * 12 + 4;
		new_vertices[edgeMap[f * 3 + 2]].halfEdge = f * 12 + 2;
	}
}

void Subdivision::createHalfEdgeInfo()
{
	new_halfEdges.clear();
	new_halfEdges.reserve(12 * faces.size());

	for (GLint i = 0; i < new_halfEdges.capacity(); ++i) {
		new_halfEdges.push_back({});
	}

	for (GLint i = 0; i < faces.size(); ++i) {
		
		GLint b = 12 * i;
		
		new_halfEdges[b] = {nIndex((i * 3) + 2, 1)};
		new_halfEdges[b + 1] = {nIndex((i * 3), 0)};
		new_halfEdges[b + 2] = {b+9};

		new_halfEdges[b + 3] = {nIndex((i * 3), 1)};
		new_halfEdges[b + 4] = {nIndex((i * 3) + 1, 0)};
		new_halfEdges[b + 5] = {b + 10};

		new_halfEdges[b + 6] = {nIndex((i * 3) + 1, 1)};
		new_halfEdges[b + 7] = {nIndex((i * 3) + 2, 0)};
		new_halfEdges[b + 8] = {b + 11};

		new_halfEdges[b + 9] = {b + 2};
		new_halfEdges[b + 10] = {b + 5};
		new_halfEdges[b + 11] = {b + 8};

	}

}

GLint Subdivision::nIndex(GLint ei, GLint type)
{
	GLint offsets[] = {3, 1, 6, 4, 0, 7};

	GLint op = halfEdges[ei].pair;

	GLint bp = 12 * (op / 3);

	return bp + offsets[2 * (op % 3) + type];

}

void keyPressed(GLFWwindow * windows, GLint key, GLint scanCode, GLint action, GLint mods) {
	if (action == GLFW_PRESS || GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_LEFT:
			//std::cerr << "move left" << std::endl;
			cam.move(Subdivision::DELTA_ANGLE * (Camera::PI/180), 0, 0);
			break;
		case GLFW_KEY_UP:
			//std::cerr << "move up" << std::endl;
			cam.move(0, Subdivision::DELTA_HEIGHT, 0);
			break;
		case GLFW_KEY_DOWN:
			//std::cerr << "move down" << std::endl;
			cam.move(0, -Subdivision::DELTA_HEIGHT, 0);
			break;
		case GLFW_KEY_RIGHT:
			//std::cerr << "move right" << std::endl;
			cam.move(-Subdivision::DELTA_ANGLE * (Camera::PI/180), 0, 0);
			break;
		case GLFW_KEY_KP_ADD:
			//std::cerr << "move in" << std::endl;
			cam.move(0, 0, Subdivision::DELTA_DEEP);
			break;
		case GLFW_KEY_KP_SUBTRACT:
			//std::cerr << "move out" << std::endl;
			cam.move(0, 0, -Subdivision::DELTA_DEEP);
			break;
		}
	}

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_SPACE:
			Subdivision::subdivision();
			break;
		case GLFW_KEY_F:
			Camera::wireframeEnabled = !Camera::wireframeEnabled;
			break;
		case GLFW_KEY_L:
			std::string filename;
			std::cout << "File's name to open: " << std::endl;
			std::cin >> filename;

			if (ObjLoader::loadFile(filename)) {
				Subdivision::vertices.clear();
				Subdivision::vertices = std::move(ObjLoader::getVertices());
				
				Subdivision::halfEdges.clear();
				Subdivision::halfEdges = std::move(ObjLoader::getHalfEdges());

				Subdivision::faces.clear();
				Subdivision::faces = std::move(ObjLoader::getFaces());
			}
			else {
				std::cerr << "Problem occured during loading!" << std::endl;
			}


			break;
		}
	}
}


int main() {
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(Subdivision::SCREEN_WIDTH, Subdivision::SCREEN_HEIGHT, "Subdivision surface", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyPressed);

	/* -- Init -- */
	init();
	
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		/* -- Render -- */
		cam.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	
	/* -- Terminate -- */

	return 0;
}


