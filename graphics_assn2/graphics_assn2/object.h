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

	void setColor(const int newColor) { color = newColor; }
	void setX(float newX) { x = newX; }
	void setY(float newY) { y = newY; }
protected:
	float x, y;
	int color = 5;
	int type;	//0 = rect, 1 = character
};

bool collisionCheck(object* a, object* b);