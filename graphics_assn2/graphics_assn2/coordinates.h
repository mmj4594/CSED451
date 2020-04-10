#pragma once
/*
	\class	corrdinates

	\brief
	gluOrtho2D를 위한 world 좌표계 정의
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

	float getLeft() { return left; }
	float getRight() { return right; }
	float getBottom() { return bottom; }
	float getTop() { return top; }

protected:
	float left;
	float right;
	float bottom;
	float top;
};
