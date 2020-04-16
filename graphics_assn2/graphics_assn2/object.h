#pragma once

/*
	\class	object

	\brief
	맵 상의 오브젝트를 정의하는 클래스.

	\detail
	int color: 해당 오브젝트의 색상(위 색상코드 참고)
	int type: 오브젝트 타입(0 = rect, 1 = character)
*/
class object {
public:
	int getType() { return type; }
	int getColor() { return color; }
	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }

	void setColor(const int newColor) { color = newColor; }
	void setX(float newX) { x = newX; }
	void setY(float newY) { y = newY; }
	void setZ(float newZ) { z = newZ; }

	bool collisionCheck(object* a);

	void draw() { return; }
protected:
	float x, y, z;
	int color = 5;
	int type;	//0 = rect, 1 = character
};