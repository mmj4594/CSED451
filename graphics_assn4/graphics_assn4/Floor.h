#pragma once
#include <GL/glew.h>
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