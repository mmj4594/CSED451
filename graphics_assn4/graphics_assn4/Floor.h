#pragma once
#include <GL/glew.h>
#include <vector>

#include "matrixStack.h"

/*
	\class	Floor

	\brief
	This class defines floor object on the world.

	\detail
	float floorSize: Overall size of floor size
	float gridSize: Size of each grid on floor
	vector<vec3> floor_vertices: Vertices that make up the floor
	vector<uvec2> floor_indices: Indices of vertices
	vector<vec3> floor_color: Color(RGB) of vertices
*/
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
				floor_normals.push_back(glm::vec3(0, 0, 1));
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
	std::vector<glm::vec3> floor_normals;
	std::vector<glm::uvec2> floor_indices;
	std::vector<glm::vec3> floor_color;
};