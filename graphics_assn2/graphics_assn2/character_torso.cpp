#include <GL/glew.h>
#include "character_torso.h"
#include "colors.h"

//ȭ��� �簢���� �׷��ش�.
void character_torso::draw() {
	setPalette(BLUE);
	glBegin(GL_POLYGON);
	glVertex2f(50, 50);
	glVertex2f(50, 70);
	glVertex2f(60, 70);
	glVertex2f(60, 50);
	glEnd();
}