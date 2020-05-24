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
class test{
public:

	GLdouble* getEye() { return eye; };
	GLdouble* getReference() { return reference; };
	GLdouble* getUpVector() { return upVector; };

private:
	GLdouble eye[3];
	GLdouble reference[3];
	GLdouble upVector[3];
};