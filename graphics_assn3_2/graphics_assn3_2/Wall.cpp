#include "Wall.h"
#include "colors.h"
#include <iostream>

using namespace std;

//Draw rect object on the screen.
void Wall::draw() {
	setPalette(color);
	switch (shape) {
	case 0:		//Cone
		glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(-90, 1, 0, 0);
		glutSolidCone(height/2, height, 10, 10);
		glPopMatrix();
		break;
	case 1:		//Torus
		glPushMatrix();
		glTranslatef(x, y+height/2, z);
		glRotatef(-90, 0, 1, 0);
		glutSolidTorus(5, height/2, 10, 10);
		glPopMatrix();
		break;
	case 2:		//Reversed T-shape
		DrawCuboid(x, y + height / 2, z, width, height, 5);
		DrawCuboid(x, y + 2.5, z, width, 5, height);
		break;
	case 3:		//T-shape
		DrawCuboid(x, y+height/2, z, width, height, 5);
		DrawCuboid(x, y + height, z, width, 5, height);
		break;
	case 5:		//Default
		DrawCuboid(x, y + height / 2, z, width, height, height);
		break;
	}
	
}

//Draw cuboid according to position and width(x), height(y), depth(z)
void DrawCuboid(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat widthLength, GLfloat heightLength, GLfloat depthLength)
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