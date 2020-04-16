#pragma once
#include "object.h"
#include "treeNode.h"
static float distancePerFrame = 0.1f;

/*
	\class	character

	\brief
	맵 상의 캐릭터를 정의하는 클래스.

	\detail
	float x: 캐릭터의 중심(torso)의 x좌표
	float y: 캐릭터의 중심(torso)의 y좌표
	float z: 캐릭터의 중심(torso)의 z좌표
	double rad: 원의 반지름 길이
	bool isCollided: 벽과의 충돌 여부 판단
*/
class character : public object {
public:
	character(float a, float b);

	double getRad() { return rad; }
	void setRad(double newRad) { rad = newRad; }

	bool getCollided() { return isCollided; }
	void setCollided() { isCollided = true; }
	void resetCollided() { isCollided = false; }

	void moveRight() { x += distancePerFrame; }

	void draw();

private:
	double rad = 5.0;
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
};

void drawHead(float x, float y, float z, float theta);
void drawLimb(float x, float y, float z, float theta);
void drawTorso(float x, float y, float z, float theta);