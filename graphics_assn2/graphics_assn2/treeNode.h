#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

/*
	\class	object

	\brief
	Left child right sibling tree를 구현하기 위한 node

	\detail
	mat4 mtx: 4*4 행렬이다.
	void (*func): 이 node에서 실행할 함수이다.
	treeNode* sibling: LCRS tree의 sibling에 해당한다.
	treeNode* child: LCRS tree의 child에 해당한다.
*/
class treeNode {
public:
	glm::mat4 mtx;
	void (*func) ();
	treeNode* sibling;
	treeNode* child;
};