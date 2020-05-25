#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <algorithm>
#include <vector>

#include "main.h"
#include "colors.h"
#include "matrixStack.h"

//Key value for space
#define SPACE 32
//All pass/fail cheat activation status
static bool allPass = false;
static bool allFail = false;

//Game status
int gameStatus = IDLE;

using namespace std;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("assn3_2");
	glutDisplayFunc(display3D);
	glutReshapeFunc(reshape3D);
	glutTimerFunc(0, frameAction, 1);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);


	init();
	glutMainLoop();

	//For safe unbound of VBO, VAO, EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);			//VBO
	glBindVertexArray(0);						//VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	//EBO

	//deallocate all resources when program ends
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &positionVBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	return 0;
}

void init() {
	glewInit();
	//Adding vertex, fragment shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar vShaderFile[] = "vShader.glvs";
	GLchar fShaderFile[] = "fshader.glfs";
	readShaderSource(vShaderFile, fShaderFile);

	//Shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	if (!CheckProgram(shaderProgram)) { cout << "Link Fail!\n"; }
	glDeleteShader(vertexShader);		//shader는 program 객체와 연결되면 필요x
	glDeleteShader(fragmentShader);

	/*
		VAO를 바인드 한 이후에 바인드되는 VBO, EBO들은 VAO에 저장된다.
		그리고 display 함수 내에서 glBindVertexArray(VAO) 만 수행하면
		해당 VAO에 저장된 VBO 및 EBO가 함께 불러와진다.
	*/
	

	//Initial Camera Setting
	setCamera(TPV);
}

//Read shader file from 'vShaderFile', 'fShaderFile' and compile them.
void readShaderSource(const char* vShaderFile, const char* fShaderFile) {
	ifstream v(vShaderFile);
	ifstream f(fShaderFile);
	if (!v.is_open() || !f.is_open()) { cout << "Shader file open Fail\n"; return; }

	string vRawString = string(istreambuf_iterator<char>(v), istreambuf_iterator<char>());
	string fRawString = string(istreambuf_iterator<char>(f), istreambuf_iterator<char>());
	const char* vSource = vRawString.c_str();
	const char* fSource = fRawString.c_str();
	glShaderSource(vertexShader, 1, &vSource, NULL);
	glShaderSource(fragmentShader, 1, &fSource, NULL);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
}

//Check whether shader program works well
bool CheckProgram(GLuint program) {
	GLint state;
	glGetProgramiv(program, GL_LINK_STATUS, &state);
	if (GL_FALSE == state) {
		int infologLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 1) {
			int charsWritten = 0;
			char* infoLog = new char[infologLength];
			glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
			std::cout << infoLog << std::endl;
			delete[] infoLog;
		}
		return false;
	}
	return true;
}

void display3D() {
	//writeLife(lifeX, lifeY);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*	Wireframe mode or Filling mode	*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//VBO
	glGenBuffers(1, &positionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);

	//EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	//우리의 shader program에서 uniform 변수의 위치를 얻음.
	int uniformProjection = glGetUniformLocation(shaderProgram, "projection");
	int uniformModelView = glGetUniformLocation(shaderProgram, "modelView");

	//projection
	mtxProj = glm::perspective(glm::radians(fovy), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 2000.0f);
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(mtxProj));
	//modelView
	mtxView = glm::lookAt(glm::vec3(eye[0], eye[1], eye[2]),
						glm::vec3(reference[0], reference[1], reference[2]),
						glm::vec3(upVector[0], upVector[1], upVector[2])
		);
	
	worldFloor.draw();
	player.draw();
	thief.draw();
	wall.draw();
	glutSwapBuffers();
}

//Action when window of the game reshaped(3D)
void reshape3D(int w, int h) {
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;
	glClearColor(1, 1, 1, 1);
	glViewport(0, 0, w, h);
	mtxProj = glm::perspective(glm::radians(fovy), (float)w / h, 1.0f, 2000.0f);
	glutPostRedisplay();
}

//Action per frame(60FPS currently)
void frameAction(int value) {
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
	if ((wall.getX() + wall.getWidth() < 0) && isPassed && (player.getY() == PLAYER_DEFAULT_Y)) {
		wallSpeed += wallSpeedIncrement;
		posePeriod -= 5;
		player.setnewX(player.getX() + player.getMovingDistance());

		newFovy = fovy * 0.99;
		fovyPerFrame = (newFovy - fovy) / zoomFrame;
		isPassed = false;
	}
	/*
		Zoom out camera if thief doesn't jump and player ignore the wall by jump
		after the wall disappear and player land successfully
	*/
	else if ((wall.getX() + wall.getWidth() < 0) && isJumped && wall.getColor() != GRAY && (player.getY() == PLAYER_DEFAULT_Y)) {
		newFovy = fovy * 1.05;
		fovyPerFrame = (newFovy - fovy) / zoomFrame;
		isJumped = false;
	}

	//Camera manipulation
	FPV = camera(player.getX() + 3, player.getY() + 8, 0,
		thief.getX(), player.getY() + 8, 0,
		0, 1, 0);
	switch (cameraMode) {
	case 1:	setCamera(FPV); break;
	case 3:	setCamera(TPV);	break;
	case 9: setCamera(XYPlane); break;
	}
	if (abs(newFovy - fovy) > 0.01) { fovy += fovyPerFrame; }

	//Ask if thief will jump
	if ((wall.getX() - thief.getX() < 35) && !askJump) {
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
	if (wall.getX() + wall.getWidth() + wallSpeed * 20 < 0) {
		int shape = wall.getShape();
		wall = Wall(WORLD_SIZE_X, 20, 0, 10, wallHeight, 0);
		wall.setShape(shape);
		thief.resetCollided();
		player.resetCollided();
		askJump = false;
		thiefJumped = false;
		wall.setShape(5);
	}
	return status;
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
		if (cameraMode != 1) { fovy *= 2; newFovy *= 2; }
		setCameraMode(1);
		break;
	case '3':
		if (cameraMode == 1) { fovy /= 2; newFovy /= 2; }
		setCameraMode(3);
		break;
	case '9':
		if (cameraMode == 1) { fovy /= 2; newFovy /= 2; }
		setCameraMode(9);
		break;
	}

	//Animation loop for player and thief
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

//Set camera to cameraPos
void setCamera(camera cameraPos) {
	memcpy(eye, cameraPos.getEye(), sizeof(eye));
	memcpy(reference, cameraPos.getReference(), sizeof(reference));
	memcpy(upVector, cameraPos.getUpVector(), sizeof(upVector));
}

//Fnish game
void finishGame() {
	glutPostRedisplay();
	glutReshapeFunc(NULL);
	glutIdleFunc(NULL);
	glutKeyboardFunc(NULL);
	glutSpecialFunc(NULL);
}
