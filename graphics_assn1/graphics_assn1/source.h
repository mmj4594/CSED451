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
	맵 상의 rectangle을 정의하는 클래스.

	\detail
	float x: 사각형의 왼쪽 아래 점의 x좌표
	float y: 사각형의 왼쪽 아래 점의 y좌표
	float width:  사각형의 폭(가로 길이)
	float height: 사각형의 높으(세로 길이)
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
	맵 상의 캐릭터를 정의하는 클래스.

	\detail
	float x: 원의 중심의 x좌표
	float y: 원의 중심의 y좌표
	double rad: 원의 반지름 길이
	int periodFrame: 도둑의 색을 바꾸기 위한 시간 계산
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

