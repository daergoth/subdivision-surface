#include "Camera.h"

GLboolean Camera::wireframeEnabled = true;

Camera::Camera(Vector4 eye_, Vector4 up_, Vector4 light_, GLfloat dist, GLfloat s_):eye(eye_), up(up_), light(light_), distance(dist), s(s_)
{
	createSceneMatrix();
	
}

Camera::~Camera()
{
	
}

void Camera::draw()
{	
	/*
	std::vector<Face*> f_var;
	
	for (Face* f : faces) {
	  Vector tmp_p[4];
	  for (GLint i = 0; i < 4; ++i) {
	    tmp_p[i] = sceneMatrix_A * *(f->p[i]);
	  }
	  Face * a = new Face(new Vector(tmp_p[0]), new Vector(tmp_p[1]), new Vector(tmp_p[2]), new Vector(tmp_p[3]));
	  a->color[0] = f->color[0];
	  a->color[1] = f->color[1];
	  a->color[2] = f->color[2];
	  f_var.push_back(a);
	}
	
	
	for (Face* f: f_var) {
	  if (Vector::innerProduct(Vector::normalize(Vector(0,0,s,1) - f->center()), f->norm()) > 0)
	      f->to_draw = false;
	}
	
	
	f_var.erase(std::remove_if(f_var.begin(), f_var.end(), 
	  [this](Face* f){
	    return Vector::innerProduct(Vector::normalize(Vector(0,0,s,1) - f->center()), f->norm()) > 0;
	  }
	), f_var.end());
	
	std::sort(f_var.begin(), f_var.end(), 
	  [this](Face* a, Face* b) {
	    return a->center()[2] < b->center()[2];
	  }
	);
	
	
	for (Face* f : f_var) {
	  if (f->to_draw) {
	    for (GLint i = 0; i < 4; ++i) {
	      *(f->p[i]) = sceneMatrix_B * *(f->p[i]);
	    }
	  }
	}
	
	
	glClear(GL_COLOR_BUFFER_BIT);
	for (Face* f : f_var) {
	  if (!f->to_draw)
	    continue;
	    
	  glColor3f(f->color[0],f->color[1],f->color[2]);
	  glBegin(GL_QUADS);
	    for (GLint point = 0; point < 4; ++point) {
	      
	      glVertex2f(
		f->p[point]->coords[0]/f->p[point]->coords[3],
		f->p[point]->coords[1]/f->p[point]->coords[3]
	      );
	    }
	  glEnd();
	  
	  
	  glColor3f(0,0,0);
	  glLineWidth(f->lineWidth);
	  glBegin(GL_LINE_LOOP);
	    for (GLint point = 0; point < 4; ++point) {
	      glVertex2f(
		f->p[point]->coords[0]/f->p[point]->coords[3],
		f->p[point]->coords[1]/f->p[point]->coords[3]
	      );
	    }
	  glEnd();
	}	

	for (Face* f : f_var) {
		for (int i = 0; i < 4; ++i) {
			delete f->p[i];
		}

		delete f;
	}
// 	std::cerr << "draw end" << std::endl;
	*/

	std::vector<Vertex> vertices_var;
	for (Vertex v : Subdivision::vertices) {
		vertices_var.push_back({ new Vector4(sceneMatrix_A * *v.vector), v.halfEdge});
	}

	//std::vector<Face> face_var;
	std::vector<GLint> visible_faces;

	/*
	for (Face v : Subdivision::faces) {
		face_var.push_back({ { v.verts[0], v.verts[1], v.verts[2] } });
	}
	*/

	for (GLint i = 0; i < Subdivision::faces.size(); ++i) {
		if (Vector4::innerProduct((Vector4(0, 0, s, 1) - (*vertices_var[Subdivision::faces[i].verts[0]].vector + *vertices_var[Subdivision::faces[i].verts[1]].vector + *vertices_var[Subdivision::faces[i].verts[2]].vector) / 3).normalize(),
			Vector4::crossProduct(*vertices_var[Subdivision::faces[i].verts[1]].vector - *vertices_var[Subdivision::faces[i].verts[0]].vector, *vertices_var[Subdivision::faces[i].verts[2]].vector - *vertices_var[Subdivision::faces[i].verts[0]].vector))
			> 0) {

			visible_faces.push_back(i);
		}
	}

	/*
	face_var.erase(std::remove_if(face_var.begin(), face_var.end(),
		[vertices_var, this](Face f) {
			Vector4 c = (*vertices_var[f.verts[0]].vector + *vertices_var[f.verts[1]].vector + *vertices_var[f.verts[2]].vector) / 3;
			return Vector4::innerProduct( (Vector4(0, 0, s, 1) - c).normalize() ,
						Vector4::crossProduct(*vertices_var[f.verts[1]].vector - *vertices_var[f.verts[0]].vector, *vertices_var[f.verts[2]].vector - *vertices_var[f.verts[0]].vector))
				 < 0;
		}
	), face_var.end());
	*/

	/*
	visible_faces.erase(std::remove_if(visible_faces.begin(), visible_faces.end(),
		[&vertices_var, this](int f) {
		return Vector4::innerProduct((Vector4(0, 0, s, 1) - (*vertices_var[Subdivision::faces[f].verts[0]].vector + *vertices_var[Subdivision::faces[f].verts[1]].vector + *vertices_var[Subdivision::faces[f].verts[2]].vector) / 3).normalize(),
			Vector4::crossProduct(*vertices_var[Subdivision::faces[f].verts[1]].vector - *vertices_var[Subdivision::faces[f].verts[0]].vector, *vertices_var[Subdivision::faces[f].verts[2]].vector - *vertices_var[Subdivision::faces[f].verts[0]].vector))
			< 0;
	}
	), visible_faces.end());
	*/

	/*
	std::sort(face_var.begin(), face_var.end(),
		[vertices_var](Face a, Face b) {
			return (*vertices_var[a.verts[0]].vector + *vertices_var[a.verts[1]].vector + *vertices_var[a.verts[2]].vector).z
				< (*vertices_var[b.verts[0]].vector + *vertices_var[b.verts[1]].vector + *vertices_var[b.verts[2]].vector).z;
		}
	);
	*/

	std::sort(visible_faces.begin(), visible_faces.end(),
		[&vertices_var](GLint a, GLint b) {
			return (vertices_var[Subdivision::faces[a].verts[0]].vector->z + vertices_var[Subdivision::faces[a].verts[1]].vector->z + vertices_var[Subdivision::faces[a].verts[2]].vector->z)/3
				< (vertices_var[Subdivision::faces[b].verts[0]].vector->z + vertices_var[Subdivision::faces[b].verts[1]].vector->z + vertices_var[Subdivision::faces[b].verts[2]].vector->z)/3;
		}
	);

	for (GLint f : visible_faces) {

		Vector4 verts[3] = {
			sceneMatrix_B * *vertices_var[Subdivision::faces[f].verts[0]].vector,
			sceneMatrix_B * *vertices_var[Subdivision::faces[f].verts[1]].vector,
			sceneMatrix_B * *vertices_var[Subdivision::faces[f].verts[2]].vector
		};

		//std::cout << verts[0] / verts[0].h << ", " << verts[1] / verts[1].h << ", " << verts[2] / verts[2].h << std::endl;

		//std::cout << light << std::endl;
		//std::cout << Vector4::crossProduct(*Subdivision::vertices[f.verts[2]].vector - *Subdivision::vertices[f.verts[0]].vector, *Subdivision::vertices[f.verts[1]].vector - *Subdivision::vertices[f.verts[0]].vector).normalize() << std::endl;

		GLfloat lightIntensity = (Vector4::innerProduct(Vector4(light).normalize(),
			Vector4::crossProduct(*Subdivision::vertices[Subdivision::faces[f].verts[1]].vector - *Subdivision::vertices[Subdivision::faces[f].verts[0]].vector, 
				*Subdivision::vertices[Subdivision::faces[f].verts[2]].vector - *Subdivision::vertices[Subdivision::faces[f].verts[0]].vector).normalize()) + 1 ) / 2;
		
		glColor3f(2 * lightIntensity, .8 * lightIntensity, .8 * lightIntensity);
		glBegin(GL_TRIANGLES);

		glVertex2d(verts[0].x / verts[0].h, verts[0].y / verts[0].h);
		glVertex2d(verts[1].x / verts[1].h, verts[1].y / verts[1].h);
		glVertex2d(verts[2].x / verts[2].h, verts[2].y / verts[2].h);

		glEnd();

		if (wireframeEnabled) {
			glColor3f(0, 0, 0);
			glBegin(GL_LINE_LOOP);

			glVertex2d(verts[0].x / verts[0].h, verts[0].y / verts[0].h);
			glVertex2d(verts[1].x / verts[1].h, verts[1].y / verts[1].h);
			glVertex2d(verts[2].x / verts[2].h, verts[2].y / verts[2].h);

			glEnd();
		}
	}
	
	for (Vertex v: vertices_var) {
		if (v.vector != nullptr) {
			delete v.vector;
			v.vector = nullptr;
		}
	}
	
}

void Camera::move(GLfloat angle_var, GLfloat height_var, GLfloat deep_var)
{
	eye.x = distance * std::cos(angle += angle_var);
	eye.y += height_var;
	eye.z = distance * std::sin(angle += angle_var);
	distance += deep_var;
	
	createSceneMatrix();
}

void Camera::createSceneMatrix()
{
	Vector4 eyeToCenter = -eye;

	Vector4 z_var = -eyeToCenter.normalize();

	Vector4 x_var = Vector4::crossProduct(up, z_var).normalize();

	Vector4 y_var = Vector4::crossProduct(z_var, x_var);

	sceneMatrix_A = Matrix4({
		{x_var.x, x_var.y, x_var.z, -Vector4::innerProduct(x_var, eye) },
		{y_var.x, y_var.y, y_var.z, -Vector4::innerProduct(y_var, eye) },
		{z_var.x, z_var.y, z_var.z, -Vector4::innerProduct(z_var, eye) },
		{0, 0, 0, 1}
	});

	//std::cout << sceneMatrix_A << std::endl;
	
	GLfloat wx = -.5f, wy= -.5f, ww= 1.f,wh= 1.f;
	GLfloat vx = Subdivision::SCREEN_WIDTH/2-100, vy= Subdivision::SCREEN_HEIGHT/2-100, vw= 100, vh= 100;

	sceneMatrix_B = Matrix4({
		{ vw / ww, 0, 0, -wx*(vw / ww) + vx },
		{ 0, vh / wh, 0, -wy*(vh / wh) + vy },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	});

	//std::cout << sceneMatrix_B << std::endl;
	
	sceneMatrix_B.destructMultiply(Matrix4({
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, -1.f / s, 1 }
	}));
	

	//std::cout << sceneMatrix_B << std::endl;

	//std::cout << sceneMatrix_B * sceneMatrix_A << std::endl;
}


