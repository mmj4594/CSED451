#pragma once
#pragma once
#pragma once
#include "object.h"
#include <GL/glew.h>
#include <GL/freeglut.h>



/*
	\class	 rect

	\brief
	This class defines rectangle object on the world.

	\detail
	float x: x coordinate of the lower left point of this object
	float y: y coordinate of the lower left point of this object
	float z: z coordinate of the lower left point of this object
	float width: length parallel to x-axis
	float height: length parallel to y-axis
	float depth: length parallel to z-axis
*/


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

void DrawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat widthLength, GLfloat depthLength, GLfloat heightLength);

