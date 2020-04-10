#include <GL/glew.h>
#include <math.h>
#include "character.h"
#include "colors.h"

//ȭ��� ���� �׷��ش�.
void drawCircle(double centerx, double centery, double rad, int color) {
	setColor(color);
	glBegin(GL_POLYGON);
	for (float angle = 0; angle < 360; angle += 1.0) {
		glVertex2f(centerx + rad * cos(angle), centery + rad * sin(angle));
	}
	glEnd();
}