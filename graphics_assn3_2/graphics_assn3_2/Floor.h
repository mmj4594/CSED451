#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <algorithm>
#include <vector>

#include "matrixStack.h"


class Floor {
public:		
	Floor (GLfloat floorSize, GLfloat gridSize) {
		
		this->floorSize = floorSize;
		this->gridSize = gridSize;
		for (GLfloat i = -floorSize; i <= floorSize; i += gridSize) {
			floor_vertices.push_back(glm::vec3(i, 20, floorSize));
			floor_vertices.push_back(glm::vec3(i, 20, -floorSize));
			floor_vertices.push_back(glm::vec3(floorSize, 20, i));
			floor_vertices.push_back(glm::vec3(-floorSize, 20, i));
			for (int j = 0; j < 4; j++) {
				floor_color.push_back(glm::vec3(0, 0, 0));
			}
		}
		for (int i = 0; i < floor_vertices.size(); i = i + 2) {
			floor_indices.push_back(glm::uvec2(i, i + 1));
		}
		
	}		
	void draw();

private:
	float floorSize, gridSize;
	
	std::vector<glm::vec3> floor_vertices;
	std::vector<glm::uvec2> floor_indices;
	std::vector<glm::vec3> floor_color;
	
};

unsigned int indices[] = {
     3,1,2,
     3,0,1,

     4,6,7,
     4,5,6,

     8,9,10,
     8,10,11,

     12,13,14,
     12,14,15,

     16,17,19,
     17,18,19,

     20,23,21,
     21,23,22
};
