#pragma once

#include <glm/glm.hpp>

/*
	\class	object

	\brief
	This class defines node for implementing
	left child right sibling(LCR) tree.

	\detail
	mat4 mtx: 4*4 matrix to multiply basically on the right of the current model-view matrix.
	mat4 additionalTransform: Additional transform in addition to base transform.
	void (*draw): Pointer to a function that performed by this node.
	treeNode* sibling: Sibling of LCRS tree.
	treeNode* child: Child of LCRS tree.
*/
class treeNode {
public:
	glm::mat4 mtx;
	glm::mat4 additionalTransform = glm::mat4(1.0f);
	void (*draw) ();
	treeNode* sibling;
	treeNode* child;
};