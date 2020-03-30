#include <iostream>
#include "source.h"
#include <stdlib.h>
#include <time.h>

//All pass/fail ġƮ Ȱ��ȭ ����
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

//ȭ���� �׷��ش�.
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawRect(world_floor.getX(), world_floor.getY(), world_floor.getWidth(), world_floor.getHeight(), BLACK);	//floor
	drawRect(wall.getX(), wall.getY(), wall.getWidth(), wall.getHeight(), wall.getColor());					//wall
	drawCircle(player.getX(), player.getY(), player.getRad(), player.getColor());								//player
	drawCircle(thief.getX(), thief.getY(), thief.getRad(), thief.getColor());									//thief

	glutSwapBuffers();			//����ۿ� �׸��� �� �׷�����, ������ۿ� ��°�� ��ü�Ѵ�.
								//���� ���ۿ����� ����Ʈ ���� ������ ������ ���� ���ο� ������ ����ۿ� ���̰�,
								//glutSwapBuffers()�� ����Ʈ ���ۿ� �� ���۰� �ٲ��.
}

//���� â ũ�� ������ �ൿ
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WORLD_X, 0, WORLD_Y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
	���� wallSpeed�� �°� �������� ��
	���� �ٸ� ������Ʈ�� �浹�� �����Ͽ�
	�浹�� �߻��� ��� �浹 �߻� ������Ʈ�� ������ ���� �ൿ.
*/
void moveWall() {
	wall.setX(wall.getX() - 0.3 * wallSpeed);

	//���� �÷��̾��� �浹
	if (collisionCheck(&wall, &player)) {
		if (!allPass && (allFail || wall.getColor() != player.getColor())) { cout << "Fail\n"; }			//Fail �� �ൿ
		else if (!allFail && (allPass || wall.getColor() == player.getColor())) { cout << "Pass\n"; }		//Pass �� �ൿ
	}
	//���� ������ �浹
	else if (collisionCheck(&wall, &thief)) {
		wall.setColor(thief.getColor());
	}
	//�÷��̾�� ������ �浹
	else if (collisionCheck(&player, &thief)) {
		cout << "Win\n";
	}

	thief.setColor(rand() % 4);	//���� �� ���� ����
	if (wall.getX() < 0) wall = rect(WORLD_X, 20, 10, 50);	//wall�� ȭ���� ��� �� ��ġ ������

	glutPostRedisplay();
}

/*
	color�� ������ ȭ���� ĥ�� �� �ֵ���
	�ȷ�Ʈ�� color�� ������ �����Ѵ�.
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

//ȭ��� �簢���� �׷��ش�.
void drawRect(double x, double y, double width, double height, const int color) {
	setColor(color);
	glBegin(GL_POLYGON);
		glVertex2f(x, y);
		glVertex2f(x, y + height);
		glVertex2f(x + width, y + height);
		glVertex2f(x + width, y);
	glEnd();
}

//ȭ��� ���� �׷��ش�.
void drawCircle(double centerx, double centery, double rad, int color) {
	setColor(color);
	glBegin(GL_POLYGON);
		for (float angle = 0; angle < 360; angle += 1.0) {
			glVertex2f(centerx + rad * cos(angle), centery + rad * sin(angle));
		}
	glEnd();
}

//Ű���� �Է¿� ���� ġƮ�� �����Ѵ�.
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
//Ű���� �Է¿� ���� �÷��̾��� ���� �����Ѵ�.
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

//�� ������Ʈ a, b�� �浹�ϴ��� ���θ� ��ȯ�Ѵ�.
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

//void glutTimerFunc(unsigned int millis, void (*func)(int value), int value); --> millis �Ŀ� func�� ȣ���ϸ� �μ��� value�� ����.
//�Ź� ȣ��Ǵ� �� �ƴ϶� �� �ѹ��� ȣ�������, ȣ��ɶ����� ���� �ֱ⸦ ���������� ���� ����.