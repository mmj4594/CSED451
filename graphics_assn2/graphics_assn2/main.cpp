#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <string>

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "main.h"
#include "colors.h"
#include "character.h"

//Key value for space
#define SPACE 32


//All pass/fail cheat activation status
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

//Display current status on screen
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

//Action when window of the game reshaped
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(world.getLeft(), world.getRight(), world.getBottom(), world.getTop());
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
}

//Action per frame(60FPS currently)
void frameAction(int value) {
	//move wall and get current status.
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

	//Ask if thief will jump
	if ((wall.getX() - thief.getX() < 27.5) && !askJump){
		askJump = true;
		//thief jump with probability
		if (rand() % 100 < thiefJumpProbability * 100) {
			cout << "Jump Thief" << endl;
			thief.jump();
			thiefJumped = true;
		}
	}

	player.checkNewPosition();
	thief.checkNewPosition();

	
	//zoom in camera if passes
	if (world.getRight() > newWorld.getRight()) zoominCamera();
	
	//Change pose of the thief in every set period
	if (thiefFrame >= colorPeriod) {
		switch (rand() % 4) {
		case 0: thief.changePose(poseA);
		case 1: thief.changePose(poseB);
		case 2: thief.changePose(poseC);
		case 3: thief.changePose(poseD);
		}
		thiefFrame = 0;
	}
	

	//Animation loop for player and thief
	if (animationFrame >= lowerBodyPeriod) { animationFrame = 0; }
	player.lowerBodyAnimation(animationFrame, lowerBodyPeriod);
	thief.lowerBodyAnimation(animationFrame, lowerBodyPeriod);
	player.upperBodyAnimation();
	thief.upperBodyAnimation();

	//Check and move if player or thief is jumping
	//player.isJumping();
	//thief.isJumping();

	thiefFrame++; animationFrame++;
	glutPostRedisplay();
	glutTimerFunc(17, frameAction, 1);		//call timer function recursively until game ends
}

/*
	Move wall in 'wallSpeed'.
	And if there is collision between wall and other object, return the game status
	according to the type of object where the collision occurs.
*/
int moveWall() {
	wall.setX(wall.getX() - 0.3 * wallSpeed);

	//Collision between wall and player
	if (wall.collisionCheck(&player)) {
		//Fail
		if (!allPass && (allFail || wall.getColor() != player.getColor() || ((wall.getColor() == 5) && (player.getY() < PLAYER_Y + jumpCriteria)))) {
			cout << "Fail\n";
			life--;
			//Lose
			if (life <= 0) return LOSE;
		}
		//Pass
		else if (!allFail && (allPass || wall.getColor() == player.getColor() || ((wall.getColor() == 5) && (player.getY() > PLAYER_Y + jumpCriteria)))) {
			cout << "Pass\n";
			pass = true;
			/*
			wallSpeed += wallSpeedIncrement;
			colorPeriod -= 5;
			player.setnewX(player.getX() + player.getMovingDistance());
			newWorld = world + coordinatesIncrement;
			*/
		}
	}
	//Collision between wall and thief
	else if (wall.collisionCheck(&thief)) {
		if (!thiefJumped) {
			wall.setColor(thief.getColor());
		}
	}
	//Collision between player and thief
	else if (player.collisionCheck(&thief)) {
		return WIN;
	}

	//Move player to right and zoom camera if win
	if ((wall.getX() + wall.getWidth() < world.getLeft()) && pass && (player.getY() == PLAYER_Y) ) {		
		wallSpeed += wallSpeedIncrement;
		colorPeriod -= 5;
		player.setnewX(player.getX() + player.getMovingDistance());
		newWorld = world + coordinatesIncrement;
		pass = false;
	}

	//Repositioning of wall when it goes out of the screen
	if (wall.getX() + wall.getWidth() + wallSpeed*20 < world.getLeft()) {
		wall = rect(world.getRight(), 20, 10, wallHeight);
		thief.resetCollided();
		player.resetCollided();
		askJump = false;
		thiefJumped = false;
	}
	return PLAYING;
}

//Define cheat according to user keyboard input.
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
	case SPACE:
		player.jump();
		cout << "JUMP" << endl;
		break;
	}

	glutPostRedisplay();
}
//Determine color of player according to user keyboard input.
void specialkeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		player.changePose(poseA);
		break;
	case GLUT_KEY_DOWN:
		player.changePose(poseB);;
		break;
	case GLUT_KEY_LEFT:
		player.changePose(poseC);
		break;
	case GLUT_KEY_RIGHT:
		player.changePose(poseD);
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

