#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <string>

#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "colors.h"

//All pass/fail 치트 활성화 여부
bool allPass = false;
bool allFail = false;

//Game status
int gameStatus = PLAYING;

using namespace std;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(750, 500);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("assn2");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, frameAction, 1);
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
	world_floor.setColor(BLACK);
}

//화면을 그려준다.
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	player.draw();
	thief.draw();
	glLoadIdentity();
	world_floor.draw();
	wall.draw();

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

//매 frame(60FPS 기준)마다의 action
void frameAction(int value) {
	//벽을 움직이고 game status를 받아옴.
	gameStatus = moveWall();
	switch (gameStatus) {
	case LOSE:
		cout << "Lose\n";
		finishGame();
		return;
	case WIN:
		cout << "Win\n";
		finishGame();
		return;
	}

	//Move player to right if passes
	if (player.getX() < playerNewX) { player.moveRight(); }
	//zoom in camera
	if (world.getLeft() < newWorld.getLeft()) zoominCamera();

	//도둑의 색을 주기에 따라 변경
	if (thiefFrame >= colorPeriod) {
		thief.setColor(rand() % 4);
		thiefFrame = 0;
	}
	else thiefFrame++;

	glutPostRedisplay();
	glutTimerFunc(17, frameAction, 1);		//call timer function recursively until game ends
}

/*
	벽을 wallSpeed에 맞게 움직여준 후
	벽과 다른 오브젝트의 충돌을 판정하여
	충돌이 발생할 경우 충돌 발생 오브젝트의 종류에 따라 행동.
*/
int moveWall() {
	wall.setX(wall.getX() - 0.3 * wallSpeed);

	//벽과 플레이어의 충돌
	if (wall.collisionCheck(&player)) {
		//Fail
		if (!allPass && (allFail || wall.getColor() != player.getColor())) {
			cout << "Fail\n";
			life--;
			//Lose
			if (life <= 0) return LOSE;
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
	else if (wall.collisionCheck(&thief)) {
		wall.setColor(thief.getColor());
	}
	//플레이어와 도둑의 충돌
	else if (player.collisionCheck(&thief)) {
		return WIN;
	}

	//wall이 화면을 벗어날 시 위치 재조정(벽 재생성)
	if (wall.getX() + wall.getWidth() < world.getLeft()) {
		wall = rect(world.getRight(), 20, 10, 50);
		thief.resetCollided();
		player.resetCollided();
	}
	return PLAYING;
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

//Display remaining life on window
void writeLife(float x, float y) {
	//life
	glColor3f(0, 0, 0);
	glRasterPos2f(x, y);
	string s = lifeText + to_string(life);
	for (string::iterator i = s.begin(); i != s.end(); ++i) {
		char c = *i;
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}

//Zoom in camera if player successfully pass wall
void zoominCamera() {
	world = world + incrementPerFrame;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(world.getLeft(), world.getRight(), world.getBottom(), world.getTop());

	//Move position of lifeText
	frameMoved++;
	lifeX = frameMoved * incrementPerFrame.getLeft() + LIFE_X * (world.getRight() - world.getLeft()) / WORLD_X;
	lifeY = frameMoved * incrementPerFrame.getBottom() + LIFE_Y * (world.getTop() - world.getBottom()) / WORLD_Y;
}

//Fnish game
void finishGame() {
	glutPostRedisplay();
	glutReshapeFunc(NULL);
	glutIdleFunc(NULL);
	glutKeyboardFunc(NULL);
	glutSpecialFunc(NULL);
}

