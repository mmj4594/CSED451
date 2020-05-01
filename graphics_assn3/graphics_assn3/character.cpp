#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <iostream>
#include <stdlib.h>

#include "character.h"
#include "colors.h"

using namespace glm;

//Static global variables for drawing character
static const float head_rad = 3.0;
static const float torso_width = 4.0;
static const float torso_height = 10.0;
static const float limb_joint_rad = 1.5;
static const float limb_length = 6.0;

//Constructor of character class
character::character(float a, float b, pose initializedPose) {
	type = 1; x = a; y = b; z = 0; newX = a; newY = b;
	
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

	//pose initialization
	newPose = currentPose = initializedPose;
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

	lua_node.additionalTransform = rotate(mat4(1.0f), radians(currentPose.lua_angle), vec3(0, 0, 1));
	rua_node.additionalTransform = rotate(mat4(1.0f), radians(currentPose.rua_angle), vec3(0, 0, 1));
	lul_node.additionalTransform = rotate(mat4(1.0f), radians(lul_angle), vec3(0, 0, 1));
	rul_node.additionalTransform = rotate(mat4(1.0f), radians(rul_angle), vec3(0, 0, 1));
	lla_node.additionalTransform = rotate(mat4(1.0f), radians(currentPose.lla_angle), vec3(0, 0, 1));
	rla_node.additionalTransform = rotate(mat4(1.0f), radians(currentPose.rla_angle), vec3(0, 0, 1));
	lll_node.additionalTransform = rotate(mat4(1.0f), radians(lll_angle), vec3(0, 0, 1));
	rll_node.additionalTransform = rotate(mat4(1.0f), radians(rll_angle), vec3(0, 0, 1));

	//tree traversal
	glPushMatrix();
		glTranslatef(x, y, 0);
		traverse(&torso_node);
	glPopMatrix();
}

//Draw the nth frame of the lower body animation loop.
void character::lowerBodyAnimation(int currentFrame, int period) {
	//Lower body posture A->B
	if (currentFrame <= period/2) {
		lul_angle = lul_angle_a + ((lul_angle_b - lul_angle_a) / (period / 2) * currentFrame);
		rul_angle = rul_angle_a + ((rul_angle_b - rul_angle_a) / (period / 2) * currentFrame);
		lll_angle = lll_angle_a + ((lll_angle_b - lll_angle_a) / (period / 2) * currentFrame);
		rll_angle = rll_angle_a + ((rll_angle_b - rll_angle_a) / (period / 2) * currentFrame);
	}
	//Lower body posture B->A
	else {
		lul_angle = lul_angle_b + ((lul_angle_a - lul_angle_b) / (period / 2) * (currentFrame - period / 2));
		rul_angle = rul_angle_b + ((rul_angle_a - rul_angle_b) / (period / 2) * (currentFrame - period / 2));
		lll_angle = lll_angle_b + ((lll_angle_a - lll_angle_b) / (period / 2) * (currentFrame - period / 2));
		rll_angle = rll_angle_b + ((rll_angle_a - rll_angle_b) / (period / 2) * (currentFrame - period / 2));
	}
}

//Draw upper body animation to change pose
void character::upperBodyAnimation() {
	//Pose changing animation during poseChangeFrame
	if (poseFrameCheck < poseChangeFrame) {
		currentPose.lua_angle = currentPose.lua_angle + poseVariance.lua_angle / poseChangeFrame;
		currentPose.lla_angle = currentPose.lla_angle + poseVariance.lla_angle / poseChangeFrame;
		currentPose.rua_angle = currentPose.rua_angle + poseVariance.rua_angle / poseChangeFrame;
		currentPose.rla_angle = currentPose.rla_angle + poseVariance.rla_angle / poseChangeFrame;
		poseFrameCheck++;
	}
}

//Change pose of character
void character::changePose(pose inputPose) {
	//Check if inputpose is different with current pose and character is not jumping
	if (currentPose.color != inputPose.color && poseFrameCheck == poseChangeFrame) {
		newPose.lua_angle = inputPose.lua_angle;
		newPose.lla_angle = inputPose.lla_angle;
		newPose.rua_angle = inputPose.rua_angle;
		newPose.rla_angle = inputPose.rla_angle;		
		poseVariance.lua_angle = newPose.lua_angle - currentPose.lua_angle;
		poseVariance.lla_angle = newPose.lla_angle - currentPose.lla_angle;
		poseVariance.rua_angle = newPose.rua_angle - currentPose.rua_angle;
		poseVariance.rla_angle = newPose.rla_angle - currentPose.rla_angle;
		currentPose.color = inputPose.color;
		setColor(currentPose.color);
		poseFrameCheck = 0;
	}
}

void character::checkNewPosition() {
	//Move right when pass
	if (cmpf(newX, x, 1)){
		moveRight();
	}
	//Jump
	else if (cmpf(newX, x, 2)) {
		moveLeft();
	}

	//Going up
	if (cmpf(newY, y, 1)) {
		moveUp();
	}
	//Going down
	else if (cmpf(newY, y, 2)) {
		moveDown();
	}
	//Finish going up
	else {
		newY = 37.0;
	}
}

//Jump character
void character::jump() {
	//Prevent double jump
	if (!cmpf(newX, x, 2)) {
		newX = x - jump_back;
		newY = y + jump_height;
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

//Compare float variables
//Mode 1: A > B, 2: A < B, 3: A == B
bool cmpf(float A, float B, int mode) {
	if (mode == 1) {
		return (A - B > 0.005f);
	}
	else if(mode == 2){
		return (B - A > 0.005f);
	}
	else {
		return (fabs(A - B) < 0.01f);
	}
}
