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
	//glutSpecialFunc(specialkeyboard);


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
	glDeleteShader(vertexShader);		//shader�� program ��ü�� ����Ǹ� �ʿ�x
	glDeleteShader(fragmentShader);

	/*
		VAO�� ���ε� �� ���Ŀ� ���ε�Ǵ� VBO, EBO���� VAO�� ����ȴ�.
		�׸��� display �Լ� ������ glBindVertexArray(VAO) �� �����ϸ�
		�ش� VAO�� ����� VBO �� EBO�� �Բ� �ҷ�������.
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

	//�츮�� shader program���� uniform ������ ��ġ�� ����.
	int uniformProjection = glGetUniformLocation(shaderProgram, "projection");
	int uniformModelView = glGetUniformLocation(shaderProgram, "modelView");

	//projection
	mtxProj = glm::perspective(fovy, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 2000.0f);
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(mtxProj));
	//modelView
	mtxView = glm::lookAt(glm::vec3(eye[0], eye[1], eye[2]),
						glm::vec3(reference[0], reference[1], reference[2]),
						glm::vec3(upVector[0], upVector[1], upVector[2])
		);
	
	worldFloor.draw();
	player.draw();
	thief.draw();

	glutSwapBuffers();
}

//Action when window of the game reshaped(3D)
void reshape3D(int w, int h) {
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;
	glClearColor(1, 1, 1, 1);
	glViewport(0, 0, w, h);
	mtxProj = glm::perspective(fovy, (float)w / h, 1.0f, 2000.0f);
	glutPostRedisplay();
}

//Action per frame(60FPS currently)
void frameAction(int value) {
	switch (cameraMode) {
	case 1:	setCamera(FPV); break;
	case 3:	setCamera(TPV);	break;
	case 9: setCamera(XYPlane); break;
	}

	glutPostRedisplay();
	glutTimerFunc(17, frameAction, 1);		//call timer function recursively until game ends
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
		//if (cameraMode != 1) { fovy *= 2; newFovy *= 2; }
		setCameraMode(1);
		break;
	case '3':
		//if (cameraMode == 1) { fovy /= 2; newFovy /= 2; }
		setCameraMode(3);
		break;
	case '9':
		//if (cameraMode == 1) { fovy /= 2; newFovy /= 2; }
		setCameraMode(9);
		break;
	}

	glutPostRedisplay();
}

//Set camera to cameraPos
void setCamera(camera cameraPos) {
	memcpy(eye, cameraPos.getEye(), sizeof(eye));
	memcpy(reference, cameraPos.getReference(), sizeof(reference));
	memcpy(upVector, cameraPos.getUpVector(), sizeof(upVector));
}
