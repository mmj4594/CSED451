#include <GL/glew.h>
#include <math.h>
#include "character.h"
#include "colors.h"

//ȭ��� ĳ���͸� �׷��ش�.
void character::drawCharacter() {
	setPalette(color);
	glBegin(GL_POLYGON);
		for (float angle = 0; angle < 360; angle += 1.0) {
			glVertex2f(x + rad * cos(angle), y + rad * sin(angle));
		}
	glEnd();
}