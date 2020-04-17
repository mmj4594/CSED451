#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

/*
	\class	object

	\brief
	Left child right sibling tree�� �����ϱ� ���� node

	\detail
	mat4 mtx: 4*4 ����̴�.
	void (*func): �� node���� ������ �Լ��̴�.
	treeNode* sibling: LCRS tree�� sibling�� �ش��Ѵ�.
	treeNode* child: LCRS tree�� child�� �ش��Ѵ�.
*/
class treeNode {
public:
	glm::mat4 mtx;
	void (*func) ();
	treeNode* sibling;
	treeNode* child;
};