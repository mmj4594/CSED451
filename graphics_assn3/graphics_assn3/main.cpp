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
static bool allPass = false;
static bool allFail = false;

//Game status
int gameStatus = IDLE;
int cameraStatus;

using namespace std;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(750, 500);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("assn3");

	glutDisplayFunc(display3D);
	glutReshapeFunc(reshape3D);
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
	glEnable(GL_DEPTH_TEST);
	setCamera(TPV);
}

//Display current status on screen(3D)

void display3D() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2], reference[0], reference[1], reference[2], upVector[0], upVector[1], upVector[2]);

	drawAxes();
	player.draw();
	thief.draw();
	drawFloor();
	wall.draw();
	
	writeLife(lifeX, lifeY);

	glutSwapBuffers();
}

//Action when window of the game reshaped(3D)
void reshape3D(int w, int h) {
	glClearColor(1, 1, 1, 1);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / h, 1, 2000);
	glMatrixMode(GL_MODELVIEW);
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
	case PASS:
		isPassed = true;
		break;
	case JUMP:
		isJumped = true;
		break;
	case FAIL:
		isFailed = true;
		break;
	}

	/*
		Move player to right and zoom camera if pass successfully
		after the wall disappear and player land successfully
	*/
	if ((wall.getX() + wall.getWidth() < world.getLeft()) && isPassed && (player.getY() == PLAYER_DEFAULT_Y)) {
		wallSpeed += wallSpeedIncrement;
		posePeriod -= 5;
		player.setnewX(player.getX() + player.getMovingDistance());
		newWorld = world + coordinatesIncrement;
		isPassed = false;
	}
	/*
		Move camera left if both thief and player pass the wall by jump
		after the wall disappear and player land successfully
	*/
	else if ((wall.getX() + wall.getWidth() < world.getLeft()) && isJumped && wall.getColor() == GRAY && (player.getY() == PLAYER_DEFAULT_Y)) {
		newWorld = world + coordinatesMoveCameraLeft;
		isJumped = false;
	}
	/*
		Zoom out camera if thief doesn't jump and player ignore the wall by jump
		after the wall disappear and player land successfully
	*/
	else if ((wall.getX() + wall.getWidth() < world.getLeft()) && isJumped && wall.getColor() != GRAY && (player.getY() == PLAYER_DEFAULT_Y)) {
		newWorld = world + coordinatesDecrement;
		isJumped = false;
	}

	//Ask if thief will jump
	if ((wall.getX() - thief.getX() < 35) && !askJump){
		askJump = true;
		//thief jump with probability
		if (rand() % 100 < thiefJumpProbability * 100) {
			thief.jump();
			thiefJumped = true;
		}
	}
	//Change pose of the thief in every set period
	if (thiefFrame >= posePeriod) {
		switch (rand() % 4) {
		case 0: thief.changePose(poseA); break;
		case 1: thief.changePose(poseB); break;
		case 2: thief.changePose(poseC); break;
		case 3: thief.changePose(poseD); break;
		}
		thiefFrame = 0;
	}
	
	//Check new position of characters
	player.checkNewPosition();
	thief.checkNewPosition();


	//Animation loop for player and thief
	if (animationFrame >= lowerBodyPeriod) { animationFrame = 0; }
	player.lowerBodyAnimation(animationFrame, lowerBodyPeriod);
	thief.lowerBodyAnimation(animationFrame, lowerBodyPeriod);
	player.upperBodyAnimation();
	thief.upperBodyAnimation();

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
	int status = IDLE;

	wall.setX(wall.getX() - 0.3 * wallSpeed);

	//Collision between wall and player
	if (wall.collisionCheck(&player)) {
		//Jump
		if (!allFail && !allPass && player.getY() > PLAYER_DEFAULT_Y + jumpCriteria) {
			status = JUMP;
		}
		//Fail
		else if (!allPass && (allFail || wall.getColor() != player.getColor() || (wall.getColor() == GRAY && player.getColor() == GRAY))) {
			status = FAIL;
			cout << "Fail\n";
			life--;
			//Lose
			if (life <= 0) status = LOSE;
		}
		//Pass
		else if (!allFail && (allPass || wall.getColor() == player.getColor())) {
			status = PASS;
			cout << "Pass\n";
		}
	}
	//Collision between wall and thief
	else if (wall.collisionCheck(&thief)) {
		if (!thiefJumped) {
			//Set shape and color of wall
			wall.setColor(thief.getColor());
			wall.setShape(thief.getColor());
		}
	}
	//Collision between player and thief
	else if (player.collisionCheck(&thief)) {
		status = WIN;
	}

	//Repositioning of wall when it goes out of the screen
	if (wall.getX() + wall.getWidth() + wallSpeed*20 < world.getLeft()) {
		int shape = wall.getShape();
		wall = Wall(world.getRight(), 20, 0, 10, wallHeight, 0);
		wall.setShape(shape);
		thief.resetCollided();
		player.resetCollided();
		askJump = false;
		thiefJumped = false;
	}
	return status;
}

//Draw Axes in red(x), green(y), blue(z)
void drawAxes() {
	glLineWidth(3.0);
	glBegin(GL_LINES);
	setPalette(RED);
	glVertex3i(0, 0, 0);
	glVertex3i(100, 0, 0);
	setPalette(GREEN);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 100, 0);
	setPalette(BLUE);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 100);
	glEnd();
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
		break;
	case '1':
		setCamera(FPV);
		break;
	case '3':
		setCamera(TPV);
		break;
	case '9':
		setCamera(XYPlane);
		break; 
	case '0':
		setCamera(ZYPlane);
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
		player.changePose(poseB);
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

//Fnish game
void finishGame() {
	glutPostRedisplay();
	glutReshapeFunc(NULL);
	glutIdleFunc(NULL);
	glutKeyboardFunc(NULL);
	glutSpecialFunc(NULL);
}

//Draw floor
void drawFloor() {
	GLfloat floorSize = (GLfloat)300;
	GLfloat gridSize = (GLfloat)25;
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	for (GLfloat i = -floorSize; i <= floorSize; i += gridSize) {
		glVertex3f(i, 20, floorSize); glVertex3f(i, 20, -floorSize);
		glVertex3f(floorSize, 20, i); glVertex3f(-floorSize, 20, i);
	}
	glEnd();
}

//Set camera to cameraPos
void setCamera(camera cameraPos) {
	memcpy(eye, cameraPos.getEye(), sizeof(eye));
	memcpy(reference, cameraPos.getReference(), sizeof(reference));
	memcpy(upVector, cameraPos.getUpVector(), sizeof(upVector));
}


