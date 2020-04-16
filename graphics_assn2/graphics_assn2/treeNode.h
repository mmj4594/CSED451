#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

class treeNode {
public:
	glm::mat4 mtx;
	void (*func) (float x, float y, float z, float tehta);
	treeNode* sibling;
	treeNode* child;
};