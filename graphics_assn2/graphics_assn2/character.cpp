#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#include "character.h"
#include "colors.h"

using namespace glm;

//Static global variables for drawing character
static double head_rad = 3.0;
static double torso_width = 8.0;
static double torso_height = 10.0;
static double limb_joint_rad = 1.5;
static double limb_length = 6.0;

//Constructor of character class
character::character(float a, float b) {
	type = 1; x = a; y = b, z = 0;
	
	//initialization for torso
	torso_node.mtx = mat4(1.0f);
	torso_node.func = drawTorso;
	torso_node.sibling = NULL;
	torso_node.child = &head_node;

	//initialization for head
	head_node.mtx = translate(mat4(1.0f), vec3(0, 8, 0));
	head_node.func = drawHead;
	head_node.sibling = &lua_node;
	head_node.child = NULL;

	//initialization for upper limb
	lua_node.mtx = translate(mat4(1.0f), vec3(-0.5 * torso_width, 0.5 * torso_height - limb_joint_rad , 0)) * rotate(mat4(1.0f), radians(180.0f), vec3(0, 0, 1));
	lua_node.func = drawLimb;
	lua_node.sibling = &rua_node;
	lua_node.child = &lla_node;
	rua_node.mtx = translate(mat4(1.0f), vec3(0.5 * torso_width, 0.5 * torso_height - limb_joint_rad, 0));
	rua_node.func = drawLimb;
	rua_node.sibling = &lul_node;
	rua_node.child = &rla_node;
	lul_node.mtx = translate(mat4(1.0f), vec3(-0.5 * torso_width + limb_joint_rad, -0.5 * torso_height, 0)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0, 0, 1));
	lul_node.func = drawLimb;
	lul_node.sibling = &rul_node;
	lul_node.child = &lll_node;
	rul_node.mtx = translate(mat4(1.0f), vec3(0.5 * torso_width - limb_joint_rad, -0.5 * torso_height, 0)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0, 0, 1));
	rul_node.func = drawLimb;
	rul_node.sibling = NULL;
	rul_node.child = &rll_node;

	//initialization for lower limb
	lla_node.mtx = translate(mat4(1.0f), vec3(limb_length, 0, 0));
	lla_node.func = drawLimb;
	lla_node.sibling = NULL;
	lla_node.child = NULL;
	rla_node.mtx = translate(mat4(1.0f), vec3(limb_length, 0, 0));
	rla_node.func = drawLimb;
	rla_node.sibling = NULL;
	rla_node.child = NULL;
	lll_node.mtx = translate(mat4(1.0f), vec3(limb_length, 0, 0));
	lll_node.func = drawLimb;
	lll_node.sibling = NULL;
	lll_node.child = NULL;
	rll_node.mtx = translate(mat4(1.0f), vec3(limb_length, 0, 0));
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
	glTranslatef(x, y+30, 0);

	glPushMatrix();
		setPalette(color);
		torso_node.func();
		setPalette(GRAY);

		glPushMatrix();
			glMultMatrixf(glm::value_ptr(head_node.mtx));
			head_node.func();
		glPopMatrix();
		glPushMatrix();
			glMultMatrixf(glm::value_ptr(lua_node.mtx));
			lua_node.func();
			glPushMatrix();
				glMultMatrixf(glm::value_ptr(lla_node.mtx));
				lla_node.func();
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glMultMatrixf(glm::value_ptr(rua_node.mtx));
			rua_node.func();
			glPushMatrix();
				glMultMatrixf(glm::value_ptr(rla_node.mtx));
				rla_node.func();
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glMultMatrixf(glm::value_ptr(lul_node.mtx));
			lul_node.func();
			glPushMatrix();
				glMultMatrixf(glm::value_ptr(lll_node.mtx));
				lll_node.func();
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glMultMatrixf(glm::value_ptr(rul_node.mtx));
			rul_node.func();
			glPushMatrix();
				glMultMatrixf(glm::value_ptr(rll_node.mtx));
				rll_node.func();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

//Draw head of character
void drawHead() {
	glBegin(GL_POLYGON);
		for (float angle = 0; angle < 360; angle += 1.0) {
			glVertex2f(head_rad * cos(angle), head_rad * sin(angle));
		}
	glEnd();
}

//Draw limb(arm and leeg) of character
void drawLimb() {
	//joint1
	glBegin(GL_POLYGON);
		for (float angle = 0; angle < 360; angle += 1.0) {
			glVertex2f(limb_joint_rad * cos(angle), limb_joint_rad * sin(angle));
		}
	glEnd();
	//skeleton
	glBegin(GL_POLYGON);
		glVertex2f(0, -limb_joint_rad);
		glVertex2f(0, limb_joint_rad);
		glVertex2f(limb_length, limb_joint_rad);
		glVertex2f(limb_length, -limb_joint_rad);
	glEnd();
	//joint2
	glBegin(GL_POLYGON);
		for (float angle = 0; angle < 360; angle += 1.0) {
			glVertex2f(limb_length + limb_joint_rad * cos(angle), limb_joint_rad * sin(angle));
		}
	glEnd();
}

//Draw torso of character
void drawTorso() {
	glBegin(GL_POLYGON);
		glVertex2f(-0.5 * torso_width, -0.5 * torso_height);
		glVertex2f(-0.5 * torso_width, 0.5 * torso_height);
		glVertex2f(0.5 * torso_width, 0.5 * torso_height);
		glVertex2f(0.5 * torso_width, -0.5 * torso_height);
	glEnd();
}