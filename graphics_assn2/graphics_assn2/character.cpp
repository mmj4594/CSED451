#include <GL/glew.h>
#include <math.h>
#include "character.h"
#include "colors.h"

//화면상에 원을 그려준다.
void drawCircle(double centerx, double centery, double rad, int color) {
	setColor(color);
	glBegin(GL_POLYGON);
	for (float angle = 0; angle < 360; angle += 1.0) {
		glVertex2f(centerx + rad * cos(angle), centery + rad * sin(angle));
	}
	glEnd();
}