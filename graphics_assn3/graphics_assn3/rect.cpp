#include "rect.h"
#include "colors.h"

//Draw rect object on the screen.
void rect::draw() {
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

	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	for (GLfloat i = -floorSize; i <= floorSize; i += gridSize) {
		glVertex3f(i, 0, floorSize); glVertex3f(i, 0, -floorSize);
		glVertex3f(floorSize, 0, i); glVertex3f(-floorSize, 0, i);
	}
	glEnd();
}