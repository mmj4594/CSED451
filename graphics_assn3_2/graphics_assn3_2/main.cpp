#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include "main.h"
#include "colors.h"

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
	//glutTimerFunc(0, frameAction, 1);
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
	glDeleteBuffers(1, &VBO);
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
	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Linking Vertex Attributes (위치)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Linking Vertex Attributes (컬러)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	//For safe unbound of VBO, VAO, EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);			//VBO
	glBindVertexArray(0);						//VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	//EBO
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*	Wireframe mode or Filling mode	*/
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	//우리의 shader program에서 uniform 변수의 위치를 얻음.
	int m_uniProjection = glGetUniformLocation(shaderProgram, "projection");
	int m_uniView = glGetUniformLocation(shaderProgram, "modelView");

	//projection
	matProj = glm::ortho(-1, 1, -1, 1);
	glUniformMatrix4fv(m_uniProjection, 1, GL_FALSE, glm::value_ptr(matProj));
	//modelView
	matView = glm::mat4(1.0f);
	glUniformMatrix4fv(m_uniView, 1, GL_FALSE, glm::value_ptr(matView));
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glutSwapBuffers();
}

//Action when window of the game reshaped(3D)
void reshape3D(int w, int h) {
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;
	glClearColor(1, 1, 1, 1);
	glViewport(0, 0, w, h);
	matProj = glm::perspective(fovy, (float)w / h, 1.0f, 2000.0f);

	glutPostRedisplay();
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