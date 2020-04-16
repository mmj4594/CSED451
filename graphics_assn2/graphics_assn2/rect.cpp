#include <GL/glew.h>
#include "rect.h"
#include "colors.h"

//ȭ��� �簢���� �׷��ش�.
void rect::draw() {
	setPalette(color);
	glBegin(GL_POLYGON);
		glVertex2f(x, y);
		glVertex2f(x, y + height);
		glVertex2f(x + width, y + height);
		glVertex2f(x + width, y);
	glEnd();
}