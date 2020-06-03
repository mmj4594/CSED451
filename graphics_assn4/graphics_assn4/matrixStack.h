#pragma once
#include <stack>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//User-defined matrix stack
std::stack<glm::mat4> modelViewStack;
std::stack<glm::mat4> projectionStack;
glm::mat4 mtxModel = glm::mat4(1.0f);
glm::mat4 mtxView = glm::mat4(1.0f);
glm::mat4 mtxProj;

//Work similar as glPushMatrix
void pushMatrix(GLenum mode) {
	if (mode == GL_MODELVIEW) modelViewStack.push(mtxModel);
	else if (mode == GL_PROJECTION) projectionStack.push(mtxProj);
}

//Work similar as glPopMatrix
void popMatrix(GLenum mode) {
	if (mode == GL_MODELVIEW) {
		mtxModel = modelViewStack.top();
		modelViewStack.pop();
	}
	else if (mode == GL_PROJECTION) {
		mtxProj = projectionStack.top();
		projectionStack.pop();
	}
}