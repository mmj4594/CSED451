#include <iostream>
#include "character.h"
#include "matrixStack.h"

using namespace glm;
using namespace std;

//Constructor of character class
character::character(float a, float b, pose initializedPose) {
	head.set(head_rad, sectorCount, stackCount);
	joint.set(limb_joint_rad, sectorCount, stackCount);
	limb.set(limb_joint_rad, limb_joint_rad, limb_length, sectorCount, stackCount);
	torso.set(torso_width / 2, torso_width / 2, torso_height, sectorCount, stackCount);
	fillColor(5);
	type = 1; x = a; y = b; z = 0; newX = a; newY = b;
	
	//initialization for torso
	torso_node.mtx = mat4(1.0f);
	torso_node.draw = drawTorso;
	torso_node.sibling = NULL;
	torso_node.child = &head_node;
	torso_node.nodeType = 0;

	//initialization for head
	head_node.mtx = translate(mat4(1.0f), vec3(0, 0.5 * torso_height + head_rad, 0));
	head_node.draw = drawHead;
	head_node.sibling = &lua_node;
	head_node.child = NULL;
	head_node.nodeType = 1;

	//initialization for upper limb
	lua_node.mtx = translate(mat4(1.0f), vec3(0, 0.5 * torso_height - limb_joint_rad , -0.5 * torso_width)) * rotate(mat4(1.0f), radians(180.0f), vec3(0, 0, 1)) * rotate(mat4(1.0f), radians(90.0f), vec3(0, 1, 0));
	lua_node.draw = drawLimb;
	lua_node.sibling = &rua_node;
	lua_node.child = &lla_node;
	lua_node.nodeType = 2;

	rua_node.mtx = translate(mat4(1.0f), vec3(0, 0.5 * torso_height - limb_joint_rad, 0.5 * torso_width)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0, 1, 0));
	rua_node.draw = drawLimb;
	rua_node.sibling = &lul_node;
	rua_node.child = &rla_node;
	rua_node.nodeType = 2;
	lul_node.mtx = translate(mat4(1.0f), vec3(0, -0.5 * torso_height, -0.5 * torso_width + limb_joint_rad)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0, 0, 1));
	lul_node.draw = drawLimb;
	lul_node.sibling = &rul_node;
	lul_node.child = &lll_node;
	lul_node.nodeType = 2;
	rul_node.mtx = translate(mat4(1.0f), vec3(0, -0.5 * torso_height, 0.5 * torso_width - limb_joint_rad)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0, 0, 1));
	rul_node.draw = drawLimb;
	rul_node.sibling = NULL;
	rul_node.child = &rll_node;
	rul_node.nodeType = 2;

	//initialization for lower limb
	lla_node.mtx = translate(mat4(1.0f), vec3(limb_length, 0, 0));
	lla_node.draw = drawLimb;
	lla_node.sibling = NULL;
	lla_node.child = NULL;
	lla_node.nodeType = 2;
	rla_node.mtx = translate(mat4(1.0f), vec3(limb_length, 0, 0));
	rla_node.draw = drawLimb;
	rla_node.sibling = NULL;
	rla_node.child = NULL;
	rla_node.nodeType = 2;
	lll_node.mtx = translate(mat4(1.0f), vec3(limb_length, 0, 0));
	lll_node.draw = drawLimb;
	lll_node.sibling = NULL;
	lll_node.child = NULL;
	lll_node.nodeType = 2;
	rll_node.mtx = translate(mat4(1.0f), vec3(limb_length, 0, 0));
	rll_node.draw = drawLimb;
	rll_node.sibling = NULL;
	rll_node.child = NULL;
	rll_node.nodeType = 2;

	//pose initialization
	newPose = currentPose = initializedPose;
}

//LCRS tree traversal for drawing character
void character::traverse(treeNode* current) {
	if (current == NULL) return;

	pushMatrix(GL_MODELVIEW);
		mtxView *= (current->mtx * current->additionalTransform);
		switch (current->nodeType) {
			case 0:
			case 1: {current->draw(head, torso); break; }		//head, torso
			case 2: {current->draw(joint, limb); break; }		//limb
		}
		if (current->child != NULL) { traverse(current->child); }
	popMatrix(GL_MODELVIEW);
	if (current->sibling != NULL) traverse(current->sibling);
}

//Draw character object on the screen
void character::draw() {
	lua_node.additionalTransform = rotate(mat4(1.0f), radians(currentPose.lua_angle), vec3(0, 0, 1));
	rua_node.additionalTransform = rotate(mat4(1.0f), radians(currentPose.rua_angle), vec3(0, 0, 1));
	lul_node.additionalTransform = rotate(mat4(1.0f), radians(lul_angle), vec3(0, 0, 1));
	rul_node.additionalTransform = rotate(mat4(1.0f), radians(rul_angle), vec3(0, 0, 1));
	lla_node.additionalTransform = rotate(mat4(1.0f), radians(currentPose.lla_angle), vec3(0, 0, 1));
	rla_node.additionalTransform = rotate(mat4(1.0f), radians(currentPose.rla_angle), vec3(0, 0, 1));
	lll_node.additionalTransform = rotate(mat4(1.0f), radians(lll_angle), vec3(0, 0, 1));
	rll_node.additionalTransform = rotate(mat4(1.0f), radians(rll_angle), vec3(0, 0, 1));

	//tree traversal
	pushMatrix(GL_MODELVIEW);
		mtxView = translate(mtxView, vec3(x, y, 0));	
		traverse(&torso_node);
	popMatrix(GL_MODELVIEW);
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
		fillColor(currentPose.color);
		
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
		newY = 17.0;
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

void character::fillColor(int color) {
	torso.setColor(color);
	head.setColor(color);
	limb.setColor(color);
	joint.setColor(color);
}

//Draw head of character
void drawHead(Sphere head, Cylinder unused) {
	head.draw();
}

//Draw limb(arm or leg) of character
void drawLimb(Sphere joint, Cylinder limb) {
}

//Draw torso of character
void drawTorso(Sphere unused, Cylinder torso) {
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
