#include <iostream>
#include "source.h"
#include <stdlib.h>
#include <time.h>

//All pass/fail 치트 활성화 여부
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

//화면을 그려준다.
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawRect(world_floor.getX(), world_floor.getY(), world_floor.getWidth(), world_floor.getHeight(), BLACK);	//floor
	drawRect(wall.getX(), wall.getY(), wall.getWidth(), wall.getHeight(), wall.getColor());					//wall
	drawCircle(player.getX(), player.getY(), player.getRad(), player.getColor());								//player
	drawCircle(thief.getX(), thief.getY(), thief.getRad(), thief.getColor());									//thief

	glutSwapBuffers();			//백버퍼에 그림을 다 그렸으면, 전면버퍼와 통째로 교체한다.
								//더블 버퍼에서는 프런트 버퍼 내용이 나오는 동안 새로운 내용이 백버퍼에 쓰이고,
								//glutSwapBuffers()로 프런트 버퍼와 백 버퍼가 바뀐다.
}

//게임 창 크기 조절시 행동
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WORLD_X, 0, WORLD_Y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
	벽을 wallSpeed에 맞게 움직여준 후
	벽과 다른 오브젝트의 충돌을 판정하여
	충돌이 발생할 경우 충돌 발생 오브젝트의 종류에 따라 행동.
*/
void moveWall() {
	wall.setX(wall.getX() - 0.3 * wallSpeed);

	//벽과 플레이어의 충돌
	if (collisionCheck(&wall, &player)) {
		if (!allPass && (allFail || wall.getColor() != player.getColor())) { cout << "Fail\n"; }			//Fail 시 행동
		else if (!allFail && (allPass || wall.getColor() == player.getColor())) { cout << "Pass\n"; }		//Pass 시 행동
	}
	//벽과 도둑의 충돌
	else if (collisionCheck(&wall, &thief)) {
		wall.setColor(thief.getColor());
	}
	//플레이어와 도둑의 충돌
	else if (collisionCheck(&player, &thief)) {
		cout << "Win\n";
	}

	thief.setColor(rand() % 4);	//도둑 색 랜덤 설정
	if (wall.getX() < 0) wall = rect(WORLD_X, 20, 10, 50);	//wall이 화면을 벗어날 시 위치 재조정

	glutPostRedisplay();
}

/*
	color의 색으로 화면을 칠할 수 있도록
	팔레트를 color의 색으로 설정한다.
*/
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

//화면상에 사각형을 그려준다.
void drawRect(double x, double y, double width, double height, const int color) {
	setColor(color);
	glBegin(GL_POLYGON);
		glVertex2f(x, y);
		glVertex2f(x, y + height);
		glVertex2f(x + width, y + height);
		glVertex2f(x + width, y);
	glEnd();
}

//화면상에 원을 그려준다.
void drawCircle(double centerx, double centery, double rad, int color) {
	setColor(color);
	glBegin(GL_POLYGON);
		for (float angle = 0; angle < 360; angle += 1.0) {
			glVertex2f(centerx + rad * cos(angle), centery + rad * sin(angle));
		}
	glEnd();
}

//키보드 입력에 따른 치트를 정의한다.
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'c':
		allFail = false;
		allPass = true;
		cout << "All pass\n";
		break;
	case 'f':
		allPass = false;
		allFail = true;
		cout << "All fail\n";
		break;
	}

	glutPostRedisplay();
}
//키보드 입력에 따라 플레이어의 색을 설정한다.
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

//두 오브젝트 a, b가 충돌하는지 여부를 반환한다.
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