#include <iostream>
#include "source.h"
#include <stdlib.h>
#include <time.h>

bool allPass = false;
bool allFail = false;

using namespace std;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(750, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(moveWall);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);

	init();
	glutMainLoop();
	return 0;
}


void init() {
	srand(time(NULL));
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_FLAT);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawSquare(0.0, 0.0, WORLD_X, 20, BLACK);														//floor
	drawSquare(wall.getX(), wall.getY(), wall.getWidth(), wall.getHeight(), wall.getColor());		//wall
	drawCircle(player.getX(), player.getY(), player.getRad(), player.getColor());					//player
	drawCircle(thief.getX(), thief.getY(), thief.getRad(), thief.getColor());						//thief

	glutSwapBuffers();			//백버퍼에 그림을 다 그렸으면, 전면버퍼와 통째로 교체한다.
								//더블 버퍼에서는 프런트 버퍼 내용이 나오는 동안 새로운 내용이 백버퍼에 쓰이고,
								//glutSwapBuffers()로 프런트 버퍼와 백 버퍼가 바뀐다.
}

void reshape(int w, int h) {
	cout << w << " " << h << endl;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WORLD_X, 0, WORLD_Y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void moveWall() {
	wall.setX(wall.getX() - 0.3 * wallSpeed);
	if (collisionCheck(&wall, &player)) {
		if (allFail || wall.getColor() != player.getColor()) { cout << "Fail\n"; }
		else if (allPass || wall.getColor() == player.getColor()) { cout << "Clear\n"; }
	}
	else if (collisionCheck(&wall, &thief)) {
		wall.setColor(thief.getColor());
	}
	else if (collisionCheck(&player, &thief)) {
		cout << "Win\n";
	}

	thief.setColor(rand() % 4);


	if (wall.getX() < 0) wall = rect(WORLD_X, 20);

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

void drawSquare(double x, double y, double width, double height, const int color) {
	setColor(color);
	glBegin(GL_POLYGON);
		glVertex2f(x, y);
		glVertex2f(x, y + height);
		glVertex2f(x + width, y + height);
		glVertex2f(x + width, y);
	glEnd();
}

void drawCircle(double centerx, double centery, double rad, int color) {
	setColor(color);
	glBegin(GL_POLYGON);
		for (float angle = 0; angle < 360; angle += 1.0) {
			glVertex2f(centerx + rad * cos(angle), centery + rad * sin(angle));
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
		player.setColor(RED);
		break;
	case GLUT_KEY_DOWN:
		player.setColor(GREEN);
		break;
	case GLUT_KEY_LEFT:
		player.setColor(BLUE);
		break;
	case GLUT_KEY_RIGHT:
		player.setColor(YELLOW);
		break;
	}
	glutPostRedisplay();
}

bool collisionCheck(object* a, object* b) {
	//wall - player/thief collision check
	if (a->getType() == 0) {
		rect* wall = (rect*)a;
		character* ch = (character*)b;

		return ((wall->getX() < (ch->getX() + ch->getRad())) && (wall->getX() + wall->getWidth() > ch->getX()));
	}

	//player - thief collision check
	else {
		character* player = (character*)a;
		character* thief = (character*)b;

		return (player->getX() + player->getRad()) > (thief->getX() - thief->getRad());
	}

}

//void glutTimerFunc(unsigned int millis, void (*func)(int value), int value); --> millis 후에 func를 호출하며 인수로 value를 전달.
//매번 호출되는 게 아니라 딱 한번만 호출되지만, 호출될때마다 다음 주기를 가변적으로 설정 가능.