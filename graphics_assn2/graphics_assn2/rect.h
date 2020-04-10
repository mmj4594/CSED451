#pragma once
#include "object.h"

/*
	\class	 rect

	\brief
	맵 상의 rectangle을 정의하는 클래스.

	\detail
	float x: 사각형의 왼쪽 아래 점의 x좌표
	float y: 사각형의 왼쪽 아래 점의 y좌표
	float width:  사각형의 폭(가로 길이)
	float height: 사각형의 높이(세로 길이)
*/
class rect : public object {
public:
	rect(float a, float b, float w, float h) { type = 0; x = a; y = b; width = w; height = h; }

	float getWidth() { return width; }
	float getHeight() { return height; }

	void setWidth(float newWidth) { width = newWidth; }
	void setHeight(float newHeight) { height = newHeight; }

	void drawRect();

private:
	float width, height;
};