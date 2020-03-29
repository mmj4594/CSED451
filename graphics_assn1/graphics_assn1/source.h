#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>

//Color codes
const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;
const int YELLOW = 3;
const int BLACK = 4;
const int GRAY = 5;

//Size of world
const int WORLD_X = 150;
const int WORLD_Y = 100;

//Wall moving speed
int wallSpeed = 1;




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
	int color = GRAY;
	int type;	//0 = rect, 1 = character
};




/*
	\class	 rect

	\brief
	�� ���� rectangle�� �����ϴ� Ŭ����.

	\detail
	float x: �簢���� ���� �Ʒ� ���� x��ǥ
	float y: �簢���� ���� �Ʒ� ���� y��ǥ
	float width:  �簢���� ��(���� ����)
	float height: �簢���� ����(���� ����)
*/
class rect: public object {
public:
	rect(float a, float b, float w, float h) { type = 0; x = a; y = b; width = w; height = h; }

	float getWidth() { return width; }
	float getHeight() { return height; }

	void setWidth(float newWidth) { width = newWidth; }
	void setHeight(float newHeight) { height = newHeight; }

private:
	float width, height;
};




/*
	\class	character

	\brief
	�� ���� ĳ���͸� �����ϴ� Ŭ����.

	\detail
	float x: ���� �߽��� x��ǥ
	float y: ���� �߽��� y��ǥ
	double rad: ���� ������ ����
*/
class character: public object {
public:
	character(float a, float b) { type = 1; x = a; y = b; }

	double getRad() { return rad; }

	void setRad(double newRad) { rad = newRad; }

private:
	double rad = 5.0;
};




rect world_floor(0, 0, 150, 20);
rect wall(WORLD_X, 20, 10, 50);
character player(25, 25);
character thief(75, 25);

void init();
void display();
void reshape(int w, int h);
void moveWall();

void setColor(int color);
void drawSquare(double x, double y, double width, double height, const int color);
void drawCircle(double centerx, double centery, double rad, const int color);

void keyboard(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);

bool collisionCheck(object* a, object* b);