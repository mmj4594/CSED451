#pragma once
#include "object.h"
#include "treeNode.h"
static float distancePerFrame = 0.1f;

/*
	\class	character

	\brief
	�� ���� ĳ���͸� �����ϴ� Ŭ����.

	\detail
	float x: ĳ������ �߽�(torso)�� x��ǥ
	float y: ĳ������ �߽�(torso)�� y��ǥ
	float z: ĳ������ �߽�(torso)�� z��ǥ
	double rad: ���� ������ ����
	bool isCollided: ������ �浹 ���� �Ǵ�
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