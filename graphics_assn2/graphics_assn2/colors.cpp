#include <GL/glew.h>
#include "colors.h"

//color의 색으로 화면을 칠할 수 있도록 팔레트를 color의 색으로 설정한다.
void setColor(int color) {
	switch (color) {
	case RED:
		glColor3f(0.8, 0.0, 0.0);
		break;
	case GREEN:
		glColor3f(0.0, 0.8, 0.0);
		break;
	case BLUE:
		glColor3f(0.0, 0.0, 0.8);
		break;
	case YELLOW:
		glColor3f(0.9, 0.9, 0.0);
		break;
	case BLACK:
		glColor3f(0.0, 0.0, 0.0);
		break;
	case GRAY:
		glColor3f(0.3, 0.3, 0.3);
		break;
	}
}