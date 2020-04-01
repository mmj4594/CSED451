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
	drawCircle(player.getX(), player.getY(), player.getRad(), player.getColor());								//player
	drawCircle(thief.getX(), thief.getY(), thief.getRad(), thief.getColor());									//thief
	drawRect(wall.getX(), wall.getY(), wall.getWidth(), wall.getHeight(), wall.getColor());						//wall
	writeLife(lifeX, lifeY);
	glutSwapBuffers();
}

//게임 창 크기 조절시 행동
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(world.getLeft(), world.getRight(), world.getBottom(), world.getTop());
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
		//Fail
		if (!allPass && (allFail || wall.getColor() != player.getColor())) { 
			cout << "Fail\n";
			life--;
			//Lose
			if (life == 0) {
				cout << "Lose\n";
				finishGame();
			}
		}
		//Pass
		else if (!allFail && (allPass || wall.getColor() == player.getColor())) { 
			cout << "Pass\n";
			wallSpeed += wallSpeedIncrement;
			colorPeriod -= 5;
			playerNewX += movingDistance;
			newWorld = world + coordinatesIncrement;
		}
	}
	//벽과 도둑의 충돌
	else if (collisionCheck(&wall, &thief)) {
		wall.setColor(thief.getColor());
	}
	//플레이어와 도둑의 충돌
	else if (collisionCheck(&player, &thief)) {
		cout << "Win\n";
		finishGame();
	}

	//Move player to right if passes
	if (player.getX() < playerNewX) {
		player.moveRight();
	}
	//Zoom camera if passes
	if (world.getLeft() < newWorld.getLeft())
	{
		world = world + incrementPerFrame;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(world.getLeft(), world.getRight(), world.getBottom(), world.getTop());

		//Move position of lifeText
		frameMoved++;
		lifeX = frameMoved * incrementPerFrame.getLeft() + LIFE_X * (world.getRight() - world.getLeft()) / WORLD_X;
		lifeY = frameMoved * incrementPerFrame.getBottom() + LIFE_Y * (world.getTop() - world.getBottom()) / WORLD_Y;
	}

	//도둑의 색을 주기에 따라 변경
	cout << thiefFrame << " " << colorPeriod << "\n";
	if (thiefFrame >= colorPeriod) {
		thief.setColor(rand() % 4);
		thiefFrame = 0;
	}
	else thiefFrame++;

	//wall이 화면을 벗어날 시 위치 재조정(벽 재생성)
	if (wall.getX() < 0) {
		wall = rect(WORLD_X, 20, 10, 50);
		thief.resetCollided();
		player.resetCollided();
	}

	glutPostRedisplay();
}

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
}

//두 오브젝트 a, b가 충돌하는지 여부를 반환한다.
bool collisionCheck(object* a, object* b) {
	//wall - player/thief collision check
	if (a->getType() == 0) {
		rect* wall = (rect*)a;
		character* ch = (character*)b;

		//Check if collision already occured
		if ((wall->getX() < ch->getX()) && !ch->getCollided()) {
			ch->setCollided();
			return true;			
		}
		return false;
	}

	//player - thief collision check
	else {
		character* player = (character*)a;
		character* thief = (character*)b;

		return (player->getX() + player->getRad()) > (thief->getX() - thief->getRad());
	}
}

//Display remaining life on window
void writeLife(float x, float y) {
	//life
	glColor3f(0, 0, 0);
	glRasterPos2f(x, y);
	string s = lifeText + to_string(life);
	for (string::iterator i = s.begin(); i != s.end(); ++i) 
	{
		char c = *i;
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}

//Fnish game
void finishGame() {
	glutReshapeFunc(NULL);
	glutIdleFunc(NULL);
	glutKeyboardFunc(NULL);
	glutSpecialFunc(NULL);
}