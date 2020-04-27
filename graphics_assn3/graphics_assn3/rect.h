#pragma once
#include "object.h"

/*
	\class	 rect

	\brief
	This class defines rectangle object on the world.

	\detail
	float x: x coordinate of the lower left point of this object
	float y: y coordinate of the lower left point of this object
	float z: z coordinate of the lower left point of this object
	float width: width of the rectangle
	float height: height of the rectangle
*/
class rect : public object {
public:
	rect(float a, float b, float w, float h) { type = 0; x = a; y = b; z = 0; width = w; height = h; }

	float getWidth() { return width; }
	float getHeight() { return height; }

	void setWidth(float newWidth) { width = newWidth; }
	void setHeight(float newHeight) { height = newHeight; }

	void draw();

private:
	float width, height;
};