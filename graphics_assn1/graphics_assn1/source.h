#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>

const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;
const int YELLOW = 3;
const int BLACK = 4;
const int GRAY = 5;

const int WORLD_X = 150;
const int WORLD_Y = 100;
int wallSpeed = 1;

typedef struct object {
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
	int type;	//0 = wall, 1 = character
} object;

typedef struct rect: object {
public:
	rect(float a, float b) { type = 0; x = a; y = b; }

	float getWidth() { return width; }
	float getHeight() { return height; }

	void setWidth(float newWidth) { width = newWidth; }
	void setHeight(float newHeight) { height = newHeight; }

private:
	float width = 10, height = 50;
} rect;
rect wall(WORLD_X, 20);

typedef struct character: object {
public:
	character(float a, float b) { type = 1; x = a; y = b; }

	double getRad() { return rad; }

	void setRad(double newRad) { rad = newRad; }

private:
	double rad = 5.0;
} character;
character player(25, 25);
character thief(75, 25);

std::vector<glm::vec3> position = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f)
};

void init();
void display();
void reshape(int w, int h);
void moveWall();

void setColor(int color);

void drawTriangle();
void drawSquare(double x, double y, double width, double height, const int color);
void drawCircle(double centerx, double centery, double rad, const int color);

void keyboard(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);

bool collisionCheck(object* a, object* b);