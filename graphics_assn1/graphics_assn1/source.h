#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>
#include <string>

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

//Position of Player
const int PLAYER_X = 25;
const int PLAYER_Y = 25;

//Position of Thief
const int THIEF_X = 75;
const int THIEF_Y = 25;

//Wall moving speed
float wallSpeed = 1;
float wallSpeedIncrement = 0.5f;

//Thief color changing period
int colorPeriod = 50;

//Collision Check
bool wallThiefCollision = false;
bool wallPlayerCollsion = false;

//Moving distance when player pass
float playerFutureX = PLAYER_X;
float movingDistance = 5;
float distancePerFrame = 0.1f;

std::string lifeText = "Life: ";



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
	int periodFrame: ������ ���� �ٲٱ� ���� �ð� ���
*/
class character: public object {
public:
	character(float a, float b) { type = 1; x = a; y = b; }

	double getRad() { return rad; }

	void setRad(double newRad) { rad = newRad; }

	int getPeriodFrame() { return periodFrame; }

	void resetPeriodFrame() { periodFrame = 0; }

	void addPeriodFrame() { periodFrame++; }

	bool getCollided() { return collided; }

	void isCollided() { collided = true; }

	void resetCollided() { collided = false; }

	void moveRight() { x += distancePerFrame; }

	int getLife() { return life; }

	void decreaseLife() { life--; };

private:
	double rad = 5.0;
	int periodFrame = 0;
	bool collided = false;;
	int life = 4;
};




rect world_floor(0, 0, 150, 20);
rect wall(WORLD_X, 20, 10, 50);
character player(PLAYER_X, PLAYER_Y);
character thief(THIEF_X, THIEF_Y);

void init();
void display();
void reshape(int w, int h);
void moveWall();

void setColor(int color);
void drawRect(double x, double y, double width, double height, const int color);
void drawCircle(double centerx, double centery, double rad, const int color);
void writeLife();

void keyboard(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);

bool collisionCheck(object* a, object* b);

void increaseWallSpeed();

