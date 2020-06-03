#include "Wall.h"
#include "colors.h"
#include <iostream>
#include "matrixStack.h"
#include "shaderinfo.h"

using namespace std;
using namespace glm;

//Draw rect object on the screen.
void Wall::draw() {
	//cout << shape << endl;
	switch (shape) {
	case 0:		//cone
		cone.setColor(color);
		pushMatrix(GL_MODELVIEW);
			mtxModel = translate(mtxModel, vec3(x, y + 27.5 / 2, z));
			mtxModel = rotate(mtxModel, radians(-90.0f), vec3(1, 0, 0));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(mtxModel));
			cone.draw();
		popMatrix(GL_MODELVIEW);
		break;
	case 1:		//reverseCone
		reverseCone.setColor(color);
		pushMatrix(GL_MODELVIEW);
			mtxModel = translate(mtxModel, vec3(x, y + 27.5 / 2, z));
			mtxModel = rotate(mtxModel, radians(-90.0f), vec3(1, 0, 0));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(mtxModel));
			reverseCone.draw();
		popMatrix(GL_MODELVIEW);
		break;
	case 2:		//cylinder1
		cylinder1.setColor(color);
		pushMatrix(GL_MODELVIEW);
			mtxModel = translate(mtxModel, vec3(x, y + 27.5 / 2, z));
			mtxModel = rotate(mtxModel, radians(-90.0f), vec3(1, 0, 0));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(mtxModel));
			cylinder1.draw();
		popMatrix(GL_MODELVIEW);
		break;
	case 3:		//cylinder2
		cylinder2.setColor(color);
		pushMatrix(GL_MODELVIEW);
			mtxModel = translate(mtxModel, vec3(x, y + 27.5 / 2, z));
			mtxModel = rotate(mtxModel, radians(-90.0f), vec3(1, 0, 0));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(mtxModel));
			cylinder2.draw();
		popMatrix(GL_MODELVIEW);
		break;
	case 5:		//Default cylinder
		cylinder.setColor(color);
		pushMatrix(GL_MODELVIEW);
			mtxModel = translate(mtxModel, vec3(x, y+27.5/2, z));
			mtxModel = rotate(mtxModel, radians(-90.0f), vec3(1, 0, 0));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(mtxModel));
			cylinder.draw();
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