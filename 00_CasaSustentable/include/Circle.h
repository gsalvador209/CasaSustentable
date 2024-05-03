#ifndef CIRCLE_H
#define CIRCLE_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
using namespace glm;

class Circle {
public:
	Circle(float size) {
		buildVertices(size);
		buildIndices();
	}

	~Circle() {
		vertices.clear();
		indices.clear();
	}

	vector<Vertex>       vertices;
	vector<unsigned int> indices;

private:

	void buildVertices(float size) {
		for (int theta = 0; theta < 360; theta += 5) {
			Vertex  v;
			double rdeg = glm::radians((double)theta);
			v.Position.x = size * (float)cos(rdeg);
			v.Position.y = size * (float)sin(rdeg);
			v.Position.z = 0.0f;
			if (theta == 0) { v.Color.r = 0.0f; v.Color.g = 1.0f; v.Color.b = 1.0f; }
			if (theta == 120) { v.Color.r = 1.0f; v.Color.g = 0.0f; v.Color.b = 1.0f; }
			if (theta == 240) { v.Color.r = 1.0f; v.Color.g = 1.0f; v.Color.b = 0.0f; }
			v.Color.a = 1.0f;
			vertices.push_back(v);
		}

		for (int theta = 0; theta < 360; theta += 5) {
			Vertex  v;
			double rdeg = glm::radians((double)theta);
			v.Position.x = 0.0f;
			v.Position.y = 0.0f;
			v.Position.z = 0.0f;
			if (theta == 0) { v.Color.r = 0.0f; v.Color.g = 1.0f; v.Color.b = 1.0f; }
			if (theta == 120) { v.Color.r = 1.0f; v.Color.g = 0.0f; v.Color.b = 1.0f; }
			if (theta == 240) { v.Color.r = 1.0f; v.Color.g = 1.0f; v.Color.b = 0.0f; }
			v.Color.a = 1.0f;
			vertices.push_back(v);
		}
	}

	void buildIndices() {
		int numVertices2 = vertices.size() / 2.0;
		for (int i = 0; i < numVertices2; i++ ) {
			indices.push_back(i); indices.push_back(i+1); indices.push_back(numVertices2 + i);
			//cout << i << "-" << i+1 << "-" << numVertices2 + i << endl;
		}
		for (int i = 0; i < numVertices2; i++) {
			indices.push_back(numVertices2 + i); indices.push_back(numVertices2 + i + 1); indices.push_back(i+1);
			//cout << numVertices2 + i << "*" << numVertices2 + i + 1 << "*" << i + 1 << endl;
		}

		indices.push_back(0); indices.push_back(numVertices2); indices.push_back(numVertices2-1); 
		//cout << vertices.size() - 1 << "#" << 0 << "#" << numVertices2 << endl;
	}
};

#endif