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

//Size of displayed world
float WORLD_X = 150;
float WORLD_Y = 100;

//Wall moving speed
float wallSpeed = 1;
const float wallSpeedIncrement = 0.5;

//Position of Player
const int PLAYER_X = 25;
const int PLAYER_Y = 25;
float playerNewX = PLAYER_X;
//Position of Thief
const int THIEF_X = 75;
const int THIEF_Y = 25;
//Thief color changing period
int thiefFrame = 0;
int colorPeriod = 75;

//Collision Check
bool wallThiefCollision = false;
bool wallPlayerCollsion = false;
//Player moving distance if passes
float movingDistance = 5;
float distancePerFrame = 0.1f;
//Camera zoom size if passes
const float LEFT_INCREMENT = 4;
const float RIGHT_INCREMENT = -5;
const float BOTTOM_INCREMENT = 1.5;
const float TOP_INCREMENT = -4.5;
const float zoomFrame = 50;

//Displacement of life on window
int life = 4;
std::string lifeText = "Life: ";
const float LIFE_X = 10;
const float LIFE_Y = 90;
float lifeX = LIFE_X;
float lifeY = LIFE_Y;
int frameMoved = 0;

/*
	\class	corrdinates

	\brief
	gluOrtho2D�� ���� world ��ǥ�� ����
*/
class coordinates {
public:
	coordinates(float left, float right, float bottom, float top) {
		this->left = left;
		this->right = right;
		this->bottom = bottom;
		this->top = top;
	}

	coordinates operator+ (coordinates& other) {
		return coordinates(left + other.getLeft(), right + other.getRight(), bottom + other.getBottom(), top + other.getTop());
	}

	float getLeft(){ return left; }
	float getRight(){ return right; }
	float getBottom(){ return bottom; }
	float getTop(){ return top; }

protected:
	float left;
	float right;
	float bottom;
	float top;
};
coordinates world(0,WORLD_X,0,WORLD_Y);
coordinates newWorld(0,WORLD_X,0,WORLD_Y);
coordinates coordinatesIncrement(LEFT_INCREMENT, RIGHT_INCREMENT, BOTTOM_INCREMENT, TOP_INCREMENT);
coordinates incrementPerFrame(LEFT_INCREMENT/zoomFrame, RIGHT_INCREMENT / zoomFrame, BOTTOM_INCREMENT / zoomFrame, TOP_INCREMENT / zoomFrame);


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
	bool isCollided: ������ �浹 ���� �Ǵ�
*/
class character: public object {
public:
	character(float a, float b) { type = 1; x = a; y = b; }

	double getRad() { return rad; }
	void setRad(double newRad) { rad = newRad; }

	bool getCollided() { return isCollided; }
	void setCollided() { isCollided = true; }
	void resetCollided() { isCollided = false; }

	void moveRight() { x += distancePerFrame; }

private:
	double rad = 5.0;
	bool isCollided = false;
};
rect world_floor(0, 0, 150, 20);
rect wall(WORLD_X, 20, 10, 50);
character player(PLAYER_X, PLAYER_Y);
character thief(THIEF_X, THIEF_Y);


//Function definition
void init();
void display();
void reshape(int w, int h);
void moveWall();

void setColor(int color);
void drawRect(double x, double y, double width, double height, const int color);
void drawCircle(double centerx, double centery, double rad, const int color);
void writeLife(float x, float y);

void keyboard(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);

bool collisionCheck(object* a, object* b);

void movePlayerRight();
void zoomCamera();

void finishGame();
