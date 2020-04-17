#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

/*
	\class	object

	\brief
	This class defines node for implementing
	left child right sibling(LCR) tree.

	\detail
	mat4 mtx: 4*4 matrix to multiply on the right of the current model-view matrix.
	void (*func): Pointer to a function that performed by this node.
	treeNode* sibling: Sibling of LCRS tree.
	treeNode* child: Child of LCRS tree.
*/
class treeNode {
public:
	glm::mat4 mtx;
	void (*func) ();
	treeNode* sibling;
	treeNode* child;
};