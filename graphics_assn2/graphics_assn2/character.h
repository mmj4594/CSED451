#pragma once
#include "object.h"
#include "treeNode.h"
static float distancePerFrame = 0.1f;

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
	character(float a, float b);

	bool getCollided() { return isCollided; }
	void setCollided() { isCollided = true; }
	void resetCollided() { isCollided = false; }

	void moveRight() { x += distancePerFrame; }

	void traverse(treeNode* current);
	void draw();

private:
	bool isCollided = false;

	treeNode torso_node;
	treeNode head_node;
	treeNode lua_node;
	treeNode lla_node;
	treeNode rua_node;
	treeNode rla_node;
	treeNode lul_node;
	treeNode lll_node;
	treeNode rul_node;
	treeNode rll_node;

	float lua_angle = 0.0;
	float rua_angle = 0.0;
	float lul_angle = -60.0;
	float rul_angle = 60.0;
	float lla_angle = -90.0;
	float rla_angle = 90.0;
	float lll_angle = -70.0;
	float rll_angle = -45.0;
};

void drawHead();
void drawLimb();
void drawTorso();