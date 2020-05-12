#pragma once
#include <stdio.h>
#include <memory.h>
#include <GL/glew.h>

/*
	\class	camera

	\brief
	Class of camera to store eye position, reference point and upvector

	\detail
	GLdouble eye[3]:		eye position vector
	GLdouble reference[3]:	view reference point vector
	GLdouble upVector[3]:	view up vector
*/
class camera {
public:
	camera(GLdouble a, GLdouble b, GLdouble c, GLdouble d, GLdouble e, GLdouble f, GLdouble g, GLdouble h, GLdouble i) {
		eye[0] = a;
		eye[1] = b;
		eye[2] = c;
		reference[0] = d;
		reference[1] = e;
		reference[2] = f;
		upVector[0] = g;
		upVector[1] = h;
		upVector[2] = i;
	}
	GLdouble* getEye() { return eye; };
	GLdouble* getReference() { return reference; };
	GLdouble* getUpVector() { return upVector; };

private:
	GLdouble eye[3];
	GLdouble reference[3];
	GLdouble upVector[3];
};