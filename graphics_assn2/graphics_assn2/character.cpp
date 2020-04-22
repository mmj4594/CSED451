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
static float head_rad = 3.0;
static float torso_width = 4.0;
static float torso_height = 10.0;
static float limb_joint_rad = 1.5;
static float limb_length = 6.0;

//Constructor of character class
character::character(float a, float b) {
	type = 1; x = a; y = b, z = 0;
	
	//initialization for torso
	torso_node.mtx = mat4(1.0f);
	torso_node.draw = drawTorso;
	torso_node.sibling = NULL;
	torso_node.child = &head_node;

	//initialization for head
	head_node.mtx = translate(mat4(1.0f), vec3(0, 0.5 * torso_height + head_rad, 0));
	head_node.draw = drawHead;
	head_node.sibling = &lua_node;
	head_node.child = NULL;

	//initialization for upper limb
	lua_node.mtx = translate(mat4(1.0f), vec3(-0.5 * torso_width, 0.5 * torso_height - limb_joint_rad , 0)) * rotate(mat4(1.0f), radians(180.0f), vec3(0, 0, 1));
	lua_node.draw = drawLimb;
	lua_node.sibling = &rua_node;
	lua_node.child = &lla_node;
	rua_node.mtx = translate(mat4(1.0f), vec3(0.5 * torso_width, 0.5 * torso_height - limb_joint_rad, 0));
	rua_node.draw = drawLimb;
	rua_node.sibling = &lul_node;
	rua_node.child = &rla_node;
	lul_node.mtx = translate(mat4(1.0f), vec3(-0.5 * torso_width + limb_joint_rad, -0.5 * torso_height, 0)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0, 0, 1));
	lul_node.draw = drawLimb;
	lul_node.sibling = &rul_node;
	lul_node.child = &lll_node;
	rul_node.mtx = translate(mat4(1.0f), vec3(0.5 * torso_width - limb_joint_rad, -0.5 * torso_height, 0)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0, 0, 1));
	rul_node.draw = drawLimb;
	rul_node.sibling = NULL;
	rul_node.child = &rll_node;

	//initialization for lower limb
	lla_node.mtx = translate(mat4(1.0f), vec3(limb_length, 0, 0));
	lla_node.draw = drawLimb;
	lla_node.sibling = NULL;
	lla_node.child = NULL;
	rla_node.mtx = translate(mat4(1.0f), vec3(limb_length, 0, 0));
	rla_node.draw = drawLimb;
	rla_node.sibling = NULL;
	rla_node.child = NULL;
	lll_node.mtx = translate(mat4(1.0f), vec3(limb_length, 0, 0));
	lll_node.draw = drawLimb;
	lll_node.sibling = NULL;
	lll_node.child = NULL;
	rll_node.mtx = translate(mat4(1.0f), vec3(limb_length, 0, 0));
	rll_node.draw = drawLimb;
	rll_node.sibling = NULL;
	rll_node.child = NULL;
}

//LCRS tree traversal for drawing character
void character::traverse(treeNode* current) {
	if (current == NULL) return;

	glPushMatrix();
		glMultMatrixf(value_ptr(current->mtx * current->additionalTransform));
		current->draw();
		if (current->child != NULL) traverse(current->child);
	glPopMatrix();

	if (current->sibling != NULL) traverse(current->sibling);
}

//Draw character object on the screen
void character::draw() {
	setPalette(color);

	lua_node.additionalTransform = rotate(mat4(1.0f), radians(lua_angle), vec3(0, 0, 1));
	rua_node.additionalTransform = rotate(mat4(1.0f), radians(rua_angle), vec3(0, 0, 1));
	lul_node.additionalTransform = rotate(mat4(1.0f), radians(lul_angle), vec3(0, 0, 1));
	rul_node.additionalTransform = rotate(mat4(1.0f), radians(rul_angle), vec3(0, 0, 1));
	lla_node.additionalTransform = rotate(mat4(1.0f), radians(lla_angle), vec3(0, 0, 1));
	rla_node.additionalTransform = rotate(mat4(1.0f), radians(rla_angle), vec3(0, 0, 1));
	lll_node.additionalTransform = rotate(mat4(1.0f), radians(lll_angle), vec3(0, 0, 1));
	rll_node.additionalTransform = rotate(mat4(1.0f), radians(rll_angle), vec3(0, 0, 1));

	//tree traversal
	glLoadIdentity();
	glPushMatrix();
		glTranslatef(x, y, 0);
		traverse(&torso_node);
	glPopMatrix();
}

//Draw the nth frame of the lower body animation loop.
void character::lowerBodyAnimation(int currentFrame, int period) {
	//A->B
	if (currentFrame <= period/2) {
		lul_angle = lul_angle_a + ((lul_angle_b - lul_angle_a) / (period / 2) * currentFrame);
		rul_angle = rul_angle_a + ((rul_angle_b - rul_angle_a) / (period / 2) * currentFrame);
		lll_angle = lll_angle_a + ((lll_angle_b - lll_angle_a) / (period / 2) * currentFrame);
		rll_angle = rll_angle_a + ((rll_angle_b - rll_angle_a) / (period / 2) * currentFrame);
	}
	//B->A
	else {
		lul_angle = lul_angle_b + ((lul_angle_a - lul_angle_b) / (period / 2) * (currentFrame - period / 2));
		rul_angle = rul_angle_b + ((rul_angle_a - rul_angle_b) / (period / 2) * (currentFrame - period / 2));
		lll_angle = lll_angle_b + ((lll_angle_a - lll_angle_b) / (period / 2) * (currentFrame - period / 2));
		rll_angle = rll_angle_b + ((rll_angle_a - rll_angle_b) / (period / 2) * (currentFrame - period / 2));
	}
}

//Draw head of character
void drawHead() {
	glBegin(GL_POLYGON);
		for (float angle = 0; angle < 360; angle += 1.0) {
			glVertex2f(head_rad * cos(angle), head_rad * sin(angle));
		}
	glEnd();
}

//Draw limb(arm or leg) of character
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
		glVertex2f(-0.5f * torso_width, -0.5f * torso_height);
		glVertex2f(-0.5f * torso_width, 0.5f * torso_height);
		glVertex2f(0.5f * torso_width, 0.5f * torso_height);
		glVertex2f(0.5f * torso_width, -0.5f * torso_height);
	glEnd();
}