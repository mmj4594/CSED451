#include <GL/glew.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#include "character.h"
#include "colors.h"

using namespace glm;

character::character(float a, float b) {
	type = 1; x = a; y = b, z = 0;
	
	torso_node.mtx = mat4(1.0f);
	torso_node.func = drawTorso;
	torso_node.sibling = NULL;
	torso_node.child = &head_node;

	head_node.mtx = translate(mat4(1.0f), vec3(0, 8, 0));
	head_node.func = drawHead;
	head_node.sibling = &lua_node;
	head_node.child = NULL;

	//lua_node.mtx = glm::mat4(1.0f);
	lua_node.func = drawLimb;
	lua_node.sibling = &rua_node;
	lua_node.child = &lla_node;
	//rua_node.mtx = glm::mat4(1.0f);
	rua_node.func = drawLimb;
	rua_node.sibling = &lul_node;
	rua_node.child = &rla_node;
	//lul_node.mtx = glm::mat4(1.0f);
	lul_node.func = drawLimb;
	lul_node.sibling = &rul_node;
	lul_node.child = &lll_node;
	//rul_node.mtx = glm::mat4(1.0f);
	rul_node.func = drawLimb;
	rul_node.sibling = NULL;
	rul_node.child = &rll_node;

	//lla_node.mtx = glm::mat4(1.0f);
	lla_node.func = drawLimb;
	lla_node.sibling = NULL;
	lla_node.child = NULL;
	//rla_node.mtx = glm::mat4(1.0f);
	rla_node.func = drawLimb;
	rla_node.sibling = NULL;
	rla_node.child = NULL;
	//lll_node.mtx = glm::mat4(1.0f);
	lll_node.func = drawLimb;
	lll_node.sibling = NULL;
	lll_node.child = NULL;
	//rll_node.mtx = glm::mat4(1.0f);
	rll_node.func = drawLimb;
	rll_node.sibling = NULL;
	rll_node.child = NULL;
}

//캐릭터를 그려주기 위한 LCRS 트리를 순환한다.
void character::traverse(treeNode* current) {
	if (current == NULL) return;

}



//화면상에 캐릭터를 그려준다.
void character::draw() {
	setPalette(color);

	//tree traversal
	glLoadIdentity();
	glTranslatef(x, y, 0);

	glPushMatrix();
		torso_node.func();

		glPushMatrix();
			glMultMatrixf(glm::value_ptr(head_node.mtx));
			head_node.func();
		glPopMatrix();
	glPopMatrix();
}

void drawHead() {
	//character_head 오브젝트를 선언하고, 그걸 그려줌.
	glBegin(GL_POLYGON);
		for (float angle = 0; angle < 360; angle += 1.0) {
			glVertex2f(3.0 * cos(angle), 3.0 * sin(angle));
		}
	glEnd();
}
void drawLimb() {
	//character_limb 오브젝트를 선언하고, 그걸 그려줌.
}
void drawTorso() {
	//character_torso 오브젝트를 선언하고, 그걸 그려줌.'
	glBegin(GL_POLYGON);
		glVertex2f(-4, -5);
		glVertex2f(-4, 5);
		glVertex2f(4, 5);
		glVertex2f(4, -5);
	glEnd();
}