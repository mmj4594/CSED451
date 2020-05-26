#pragma once
#include "object.h"
#include "Cylinder.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

const float wallGridSize = 25;
const float wallFloorSize = 500;

class Wall : public object {
public:
	Wall(float a, float b, float c, float w, float h, float d) {
		type = 0;
		x = a; y = b; z = c;
		width = w; height = h; depth = d;
	}

	float getWidth() { return width; }
	float getHeight() { return height; }
	float getDepth() { return depth; }
	float getShape() { return shape; }

	void setWidth(float newWidth) { width = newWidth; }
	void setHeight(float newHeight) { height = newHeight; }
	void setDepth(float newDepth) { depth = newDepth; }
	void setShape(float newShape) { shape = newShape; }

	void draw();

private:
	float width, height, depth;
	int shape = 5;
	
};

Cylinder cone((float)0, 27.5/2, 27.5, 16, 16);
Cylinder reverseCone(27.5 / 2, (float) 0, 27.5, 16, 16);
Cylinder cylinder(27.5 / 2, 27.5 / 2, 27.5, 16, 16);
Cylinder cylinder1(27.5 / 4, 27.5 / 2, 27.5, 16, 16);
Cylinder cylinder2(27.5 / 2, 27.5 / 4, 27.5, 16, 16);
void DrawCuboid(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat widthLength, GLfloat depthLength, GLfloat heightLength);

