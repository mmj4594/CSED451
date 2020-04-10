#pragma once
#include "object.h"

static float distancePerFrame = 0.1f;

/*
	\class	character

	\brief
	맵 상의 캐릭터를 정의하는 클래스.

	\detail
	float x: 원의 중심의 x좌표
	float y: 원의 중심의 y좌표
	double rad: 원의 반지름 길이
	bool isCollided: 벽과의 충돌 여부 판단
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