#include <iostream>
#include "source.h"

bool allPass = false;
bool allFail = true;

using namespace std;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(moveObjects);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);

	init();
	glutMainLoop();
	return 0;
}


void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_FLAT);
	rectangle.x = rectangle.y = 0.0f;
	rectangle.width = 0.1;
	rectangle.height = 0.15;
	
	glBegin(GL_POLYGON);
		glVertex2f(0, 0);
		glVertex2f(0, 250);
		glVertex2f(250, 250);
		glVertex2f(250, 0);
	glEnd();
	
}

void display() {
	drawTriangle();
	drawSquare(rectangle.x, rectangle.y, rectangle.width, rectangle.height);
	drawCircle(0.5 ,0.5, GREEN);

	glutSwapBuffers();			//백버퍼에 그림을 다 그렸으면, 전면버퍼와 통째로 교체한다.
								//더블 버퍼에서는 프런트 버퍼 내용이 나오는 동안 새로운 내용이 백버퍼에 쓰이고,
								//glutSwapBuffers()로 프런트 버퍼와 백 버퍼가 바뀐다.
}

void reshape(int w, int h) {
	cout << w << " " << h << endl;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void moveObjects() {
	if(rectangle.x + rectangle.width < 1) rectangle.x += 0.001;
	if(rectangle.y + rectangle.height < 1) rectangle.y += 0.001;

	glutPostRedisplay();
}

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
		glColor3f(1.0, 1.0, 0.0);
		break;
	case BLACK:
		glColor3f(0.0, 0.0, 0.0);
	}
}

void drawTriangle() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
		for (int i = 0; i < position.size(); i++) {
			glVertex3f(position[i][0], position[i][1], position[i][2]);
		}
	glEnd();
}

void drawSquare(double x, double y, double width, double height) {
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x, y + height);
		glVertex2f(x + width, y + height);
		glVertex2f(x + width, y);
	glEnd();
}

void drawCircle(double x, double y, int color) {
	setColor(color);
	double rad = 0.05;
	glBegin(GL_POLYGON);
		for (float angle = 0; angle < 360; angle += 1.0) {
			glVertex2f(x + rad * cos(angle), y + rad * sin(angle));
		}
	glEnd();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'c':
		allFail = false;
		allPass = true;
		break;
	case 'f':
		allPass = false;
		allFail = true;
		break;
	}

	glutPostRedisplay();
}
void specialkeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		rectangle.y += 0.005;
		break;
	case GLUT_KEY_DOWN:
		rectangle.y -= 0.005;
		break;
	case GLUT_KEY_LEFT:
		rectangle.x -= 0.005;
		break;
	case GLUT_KEY_RIGHT:
		rectangle.x += 0.005;
		break;
	}
	glutPostRedisplay();
}

//void glutTimerFunc(unsigned int millis, void (*func)(int value), int value); --> millis 후에 func를 호출하며 인수로 value를 전달.
//매번 호출되는 게 아니라 딱 한번만 호출되지만, 호출될때마다 다음 주기를 가변적으로 설정 가능.