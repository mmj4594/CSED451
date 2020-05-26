#include <glm/gtc/type_ptr.hpp>

#include "Floor.h"
#include "main.h"

void Floor::draw() {	
	glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
	glBufferData(GL_ARRAY_BUFFER, floor_vertices.size() * sizeof(glm::vec3), glm::value_ptr(floor_vertices[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, floor_color.size() * sizeof(glm::vec3), glm::value_ptr(floor_color[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, floor_indices.size() * sizeof(glm::uvec2), glm::value_ptr(floor_indices[0]), GL_STATIC_DRAW);

	glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(mtxView));
	glDrawElements(GL_LINES, floor_vertices.size(), GL_UNSIGNED_INT, 0);
}