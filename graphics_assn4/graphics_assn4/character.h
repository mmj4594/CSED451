#pragma once
#include "object.h"
#include "treeNode.h"
#include "pose.h"
#include "colors.h"
#include "Sphere.h"
#include "Cylinder.h"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>

//Variables for player to move right when passes
static float movingFrame = 50.0;
static float movingDistance = 10.0;
static float distancePerFrame = movingDistance/movingFrame;

//Variables for player or thief to jump
static float jump_height = 40.0;
float jump_back = 10.0;
static float jumpFrame = 100;
static float heightPerFrame = jump_height / (jumpFrame/2);

//Static global variables for drawing character
static const float head_rad = 3.0;
static const float torso_depth = 3.0;
static const float torso_width = 6.0;
static const float torso_height = 10.0;
static const float limb_joint_rad = 1;
static const float limb_length = 6.0;

static int sectorCount = 16;
static int stackCount = 16;

//Pose Definition
pose initialPose(5, -10, -5, 10, GRAY);
pose poseA(-80, -10, 80, 10, RED);
pose poseB(10, 90, -10, -90, GREEN);
pose poseC(-10, -90, 10, 90, BLUE);
pose poseD(10, 70, 10, 70, YELLOW);

int color;

/*
	\class	character

	\brief
	This class defines character object on the world.

	\detail
	float x: x coordinate of the center of the character(torso)
	float y: y coordinate of the center of the character(torso)
	float z: z coordinate of the center of the character(torso)
	bool isCollided: Determine the collision with wall
*/
class character : public object {
public:
	character(float a, float b, pose initializedPose);

	bool getCollided() { return isCollided; }
	void setCollided() { isCollided = true; }
	void resetCollided() { isCollided = false; }

	void moveRight() { x += distancePerFrame; }
	void moveLeft() { x -= jump_back/jumpFrame; }
	void moveUp() { y += heightPerFrame; }
	void moveDown() { y -= heightPerFrame; }

	void traverse(treeNode* current);
	void draw();

	void lowerBodyAnimation(int currentFrame, int period);
	void changePose(pose inputPose);
	void upperBodyAnimation();

	void setnewX(float newX) { this->newX = newX; }
	void setnewY(float newY) { this->newY = newY; }
	float getnewX() { return newX; }
	float getnewY() { return newY; }
	float getMovingDistance(){ return movingDistance; }
	int getColor() { return currentPose.color; };

	void fillColor(int color);
	void checkNewPosition();
	void jump();
	

private:
	bool isCollided = false;
	float newX, newY = 37.0;

	treeNode torso_node;
	treeNode head_node;
	treeNode lua_node;
	treeNode rua_node;
	treeNode lul_node;
	treeNode rul_node;
	treeNode lla_node;
	treeNode rla_node;
	treeNode lll_node;
	treeNode rll_node;
 
	float lul_angle = -60.0;
	float rul_angle = 60.0;
	float lll_angle = -70.0;
	float rll_angle = -45.0;

	//for lower body animation
	float lul_angle_a = -60.0;
	float lul_angle_b = 75.0;
	float rul_angle_a = 60.0;
	float rul_angle_b = -65.0;
	float lll_angle_a = -70.0;
	float lll_angle_b = -45.0;
	float rll_angle_a = -45.0;
	float rll_angle_b = -70.0;

	//Variables to change pose
	pose currentPose;
	pose newPose;
	pose poseVariance;
	float poseChangeFrame = 30.0;
	int poseFrameCheck = 30;
	Sphere head;
	Sphere joint;
	Cylinder limb;
	Cylinder torso;
};

void drawHead(Sphere head, Cylinder unused);
void drawLimb(Sphere joint, Cylinder limb);
void drawTorso(Sphere unused, Cylinder torso);

bool cmpf(float A, float B, int mode);

