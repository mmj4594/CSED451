#pragma once
#include "object.h"

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