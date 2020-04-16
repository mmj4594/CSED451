#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

#include "character.h"
#include "character_head.h"
#include "character_limb.h"
#include "character_torso.h"
#include "colors.h"

using namespace glm;

character::character(float a, float b) {
	type = 1; x = a; y = b, z = 0;
	
	torso_node.mtx = mat4(1.0f);
	torso_node.func = drawTorso;
	torso_node.sibling = NULL;
	torso_node.child = &head_node;

	torso_node.mtx = translate(mat4(), vec3(0.0f, 8.0f, 0.0f));
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

	//lua_node.mtx = glm::mat4(1.0f);
	lla_node.func = drawLimb;
	lla_node.sibling = NULL;
	lla_node.child = NULL;
	//rua_node.mtx = glm::mat4(1.0f);
	rla_node.func = drawLimb;
	rla_node.sibling = NULL;
	rla_node.child = NULL;
	//lul_node.mtx = glm::mat4(1.0f);
	lll_node.func = drawLimb;
	lll_node.sibling = NULL;
	lll_node.child = NULL;
	//rul_node.mtx = glm::mat4(1.0f);
	rll_node.func = drawLimb;
	rll_node.sibling = NULL;
	rll_node.child = NULL;
}



//화면상에 캐릭터를 그려준다.
void character::draw() {
	setPalette(color);
	glBegin(GL_POLYGON);
		for (float angle = 0; angle < 360; angle += 1.0) {
			glVertex2f(x + rad * cos(angle), y + rad * sin(angle));
		}
	glEnd();

	//tree traversal
}

void drawHead(float x, float y, float z, float theta) {
	//character_head 오브젝트를 선언하고, 그걸 그려줌.
}
void drawLimb(float x, float y, float z, float theta) {
	//character_limb 오브젝트를 선언하고, 그걸 그려줌.
}
void drawTorso(float x, float y, float z, float theta) {
	//character_torso 오브젝트를 선언하고, 그걸 그려줌.
}