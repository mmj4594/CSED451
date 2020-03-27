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

typedef struct rect {
	float x, y;
	float width, height;
} rect;
rect rectangle;

std::vector<glm::vec3> position = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f)
};

void init();
void display();
void reshape(int w, int h);
void moveObjects();

void setColor(int color);

void drawTriangle();
void drawSquare(double x, double y, double width, double height);
void drawCircle(double x, double y, int color);

void keyboard(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);