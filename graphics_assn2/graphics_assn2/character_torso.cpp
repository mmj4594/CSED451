#include <GL/glew.h>
#include "character_torso.h"
#include "colors.h"

//화면상에 사각형을 그려준다.
void character_torso::draw() {
	setPalette(BLUE);
	glBegin(GL_POLYGON);
	glVertex2f(50, 50);
	glVertex2f(50, 70);
	glVertex2f(60, 70);
	glVertex2f(60, 50);
	glEnd();
}