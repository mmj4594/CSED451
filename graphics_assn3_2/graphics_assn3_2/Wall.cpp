#include "Wall.h"
#include "colors.h"
#include <iostream>
#include "matrixStack.h"

using namespace std;
using namespace glm;

//Draw rect object on the screen.
void Wall::draw() {
	setPalette(color);
	switch (shape) {
	case 0:		//Cone
		cout << x << endl;
		pushMatrix(GL_MODELVIEW);
			mtxView = translate(mtxView, vec3(x, y, z));
			mtxView = rotate(mtxView, -90.0f, vec3(1, 0, 0));
			glUniformMatrix4fv(2, 1, GL_FALSE, value_ptr(mtxView));
			cone.draw();
		popMatrix(GL_MODELVIEW);
		break;
	case 1:		//Torus
		pushMatrix(GL_MODELVIEW);
			mtxView = translate(mtxView, vec3(x, y + height / 2, z));
			mtxView = rotate(mtxView, -90.0f, vec3(0, 1, 0));
			glUniformMatrix4fv(2, 1, GL_FALSE, value_ptr(mtxView));
			//solid torus drawing function
		popMatrix(GL_MODELVIEW);
		break;
	case 2:		//Reversed T-shape
		//cuboid drawing function
		break;
	case 3:		//T-shape
		//cuboid drawing function
		break;
	case 5:		//Default
		pushMatrix(GL_MODELVIEW);
			mtxView = translate(mtxView, vec3(x, y, z));
			mtxView = rotate(mtxView, radians(-90.0f), vec3(1, 0, 0));
			glUniformMatrix4fv(2, 1, GL_FALSE, value_ptr(mtxView));
			cone.draw();
		popMatrix(GL_MODELVIEW);
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