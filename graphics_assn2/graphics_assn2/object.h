#pragma once

/*
	\class	object

	\brief
	�� ���� ������Ʈ�� �����ϴ� Ŭ����.

	\detail
	int color: �ش� ������Ʈ�� ����(�� �����ڵ� ����)
	int type: ������Ʈ Ÿ��(0 = rect, 1 = character)
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