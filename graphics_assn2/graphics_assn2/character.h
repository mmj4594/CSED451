#pragma once
#include "object.h"

static float distancePerFrame = 0.1f;

/*
	\class	character

	\brief
	�� ���� ĳ���͸� �����ϴ� Ŭ����.

	\detail
	float x: ���� �߽��� x��ǥ
	float y: ���� �߽��� y��ǥ
	double rad: ���� ������ ����
	bool isCollided: ������ �浹 ���� �Ǵ�
*/
class character : public object {
public:
	character(float a, float b) { type = 1; x = a; y = b; }

	double getRad() { return rad; }
	void setRad(double newRad) { rad = newRad; }

	bool getCollided() { return isCollided; }
	void setCollided() { isCollided = true; }
	void resetCollided() { isCollided = false; }

	void moveRight() { x += 0.1f; }

private:
	double rad = 5.0;
	bool isCollided = false;
};

void drawCircle(double centerx, double centery, double rad, const int color);