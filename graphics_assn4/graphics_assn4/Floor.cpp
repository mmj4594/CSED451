#include "Floor.h"
#include "colors.h"
#include <iostream>
#include "matrixStack.h"
#include "shaderinfo.h"

using namespace std;
using namespace glm;

void Floor::draw() {
	pushMatrix(GL_MODELVIEW);
	mtxModel = translate(mtxModel, vec3(0,-10,0));
	mtxModel = rotate(mtxModel, radians(-90.0f), vec3(1, 0, 0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(mtxModel));
	floorModel.draw();
	popMatrix(GL_MODELVIEW);

	/*
	glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
	glBufferData(GL_ARRAY_BUFFER, floor_vertices.size() * sizeof(glm::vec3), glm::value_ptr(floor_vertices[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(aPosLocation);
	glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, floor_color.size() * sizeof(glm::vec3), glm::value_ptr(floor_color[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(aColorLocation);
	glVertexAttribPointer(aColorLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//Normal VBO
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glBufferData(GL_ARRAY_BUFFER, floor_normals.size() * sizeof(glm::vec3), glm::value_ptr(floor_normals[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(aNormalLocation);
	glVertexAttribPointer(aNormalLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, floor_indices.size() * sizeof(glm::uvec2), glm::value_ptr(floor_indices[0]), GL_STATIC_DRAW);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(mtxModel));
	glDrawElements(GL_LINES, floor_vertices.size(), GL_UNSIGNED_INT, 0);
	*/
}