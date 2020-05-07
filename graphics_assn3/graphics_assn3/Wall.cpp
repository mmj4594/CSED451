#include "Wall.h"
#include "colors.h"
#include <iostream>

using namespace std;

//Draw rect object on the screen.
void Wall::draw() {
	/*
	for (int i = 0; i < 10; i++) {
		glPushMatrix();
		color = 4;

		glTranslatef(i*500, -250, 0);
		setPalette(color);
		glutSolidCube(500);
		glPopMatrix();
	}

	*/
	/*setPalette(color);
	glBegin(GL_POLYGON);
		glVertex2f(x, y);
		glVertex2f(x, y + height);
		glVertex2f(x + width, y + height);
		glVertex2f(x + width, y);
	glEnd();
	*/

	setPalette(color);
	switch (shape) {
	case 0:
		glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(-90, 1, 0, 0);
		glutSolidCone(height/2, height, 10, 10);
		glPopMatrix();
		break;
	case 1:
		glPushMatrix();
		glTranslatef(x, y+height/2, z);
		glRotatef(-90, 0, 1, 0);
		glutSolidTorus(5, height/2, 10, 10);
		glPopMatrix();
		break;
	case 2:
		DrawCube(x, y + height / 2, z, width, height, 5);
		DrawCube(x, y + 2.5, z, height / 2, 5, height);
		break;
	case 3:
		DrawCube(x, y+height/2, z, width, height, 5);
		DrawCube(x, y + height, z, height / 2, 5, height);
		break;
	case 5:
		DrawCube(x, y + height / 2, z, width, height, height);
		break;
	}
	
}

void DrawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat widthLength, GLfloat heightLength, GLfloat depthLength)
{
	GLfloat halfwidthLength = widthLength * 0.5f;
	GLfloat halfdepthLength = depthLength * 0.5f;
	GLfloat halfheightLength = heightLength * 0.5f;

	GLfloat vertices[] =
	{
		// front face
		centerPosX - halfwidthLength, centerPosY + halfheightLength, centerPosZ + halfdepthLength, // top left
		centerPosX + halfwidthLength, centerPosY + halfheightLength, centerPosZ + halfdepthLength, // top right
		centerPosX + halfwidthLength, centerPosY - halfheightLength, centerPosZ + halfdepthLength, // bottom right
		centerPosX - halfwidthLength, centerPosY - halfheightLength, centerPosZ + halfdepthLength, // bottom left		

		// back face
		centerPosX - halfwidthLength, centerPosY + halfheightLength, centerPosZ - halfdepthLength, // top left
		centerPosX + halfwidthLength, centerPosY + halfheightLength, centerPosZ - halfdepthLength, // top right
		centerPosX + halfwidthLength, centerPosY - halfheightLength, centerPosZ - halfdepthLength, // bottom right
		centerPosX - halfwidthLength, centerPosY - halfheightLength, centerPosZ - halfdepthLength, // bottom left	

		// left face
		centerPosX - halfwidthLength, centerPosY + halfheightLength, centerPosZ + halfdepthLength, // top left
		centerPosX - halfwidthLength, centerPosY + halfheightLength, centerPosZ - halfdepthLength, // top right
		centerPosX - halfwidthLength, centerPosY - halfheightLength, centerPosZ - halfdepthLength, // bottom right
		centerPosX - halfwidthLength, centerPosY - halfheightLength, centerPosZ + halfdepthLength, // bottom left	

		// right face
		centerPosX + halfwidthLength, centerPosY + halfheightLength, centerPosZ + halfdepthLength, // top left
		centerPosX + halfwidthLength, centerPosY + halfheightLength, centerPosZ - halfdepthLength, // top right
		centerPosX + halfwidthLength, centerPosY - halfheightLength, centerPosZ - halfdepthLength, // bottom right
		centerPosX + halfwidthLength, centerPosY - halfheightLength, centerPosZ + halfdepthLength, // bottom left	

		// top face
		centerPosX - halfwidthLength, centerPosY + halfheightLength, centerPosZ + halfdepthLength, // top left
		centerPosX - halfwidthLength, centerPosY + halfheightLength, centerPosZ - halfdepthLength, // top right
		centerPosX + halfwidthLength, centerPosY + halfheightLength, centerPosZ - halfdepthLength, // bottom right
		centerPosX + halfwidthLength, centerPosY + halfheightLength, centerPosZ + halfdepthLength, // bottom left	

		// top face
		centerPosX - halfwidthLength, centerPosY - halfheightLength, centerPosZ + halfdepthLength, // top left
		centerPosX - halfwidthLength, centerPosY - halfheightLength, centerPosZ - halfdepthLength, // top right
		centerPosX + halfwidthLength, centerPosY - halfheightLength, centerPosZ - halfdepthLength, // bottom right
		centerPosX + halfwidthLength, centerPosY - halfheightLength, centerPosZ + halfdepthLength, // bottom left	
	};

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glColor3f( colour[0], colour[1], colour[2] );
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glDrawArrays(GL_QUADS, 0, 24);

	glDisableClientState(GL_VERTEX_ARRAY);
}