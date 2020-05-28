#include <fstream>
#include <iostream>
#include "shaderinfo.h"

using namespace std;

//Initiate shader program
void initShader() {
	//Adding vertex, fragment shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar vShaderFile[] = "gouraudVshader.glvs";
	GLchar fShaderFile[] = "gouraudFshader.glfs";
	readShaderSource(vShaderFile, fShaderFile);

	//Link shader program
	shaderProgram[0] = glCreateProgram();
	glAttachShader(shaderProgram[0], vertexShader);
	glAttachShader(shaderProgram[0], fragmentShader);
	glLinkProgram(shaderProgram[0]);
	if (!CheckProgram(shaderProgram[0])) { cout << "Link Fail!\n"; }
	//Get location of variables
	aPosLocation = glGetAttribLocation(shaderProgram[0], "aPos");
	aNormalLocation = glGetAttribLocation(shaderProgram[0], "aNormal");
	aColorLocation = glGetAttribLocation(shaderProgram[0], "aColor");
	ambientProductLocation = glGetUniformLocation(shaderProgram[0], "ambientProduct");
	diffuseProductLocation = glGetUniformLocation(shaderProgram[0], "diffuseProduct");
	specularProductLocation = glGetUniformLocation(shaderProgram[0], "specularProduct");
	modelViewLocation = glGetUniformLocation(shaderProgram[0], "modelView");
	projectionLocation = glGetUniformLocation(shaderProgram[0], "projection");
	lightPositionLocation = glGetUniformLocation(shaderProgram[0], "lightPosition");
	shininessLocation = glGetUniformLocation(shaderProgram[0], "shininess");
	//shader는 program 객체와 연결되면 필요x
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*
		VAO를 바인드 한 이후에 바인드되는 VBO, EBO들은 VAO에 저장된다.
		그리고 display 함수 내에서 glBindVertexArray(VAO) 만 수행하면
		해당 VAO에 저장된 VBO 및 EBO가 함께 불러와진다.
	*/
	//VAO
	glGenVertexArrays(1, &VAO[0]);
	glBindVertexArray(VAO[0]);
	//VBO
	glGenBuffers(1, &positionVBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, positionVBO[0]);
	glGenBuffers(1, &colorVBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO[0]);
	glGenBuffers(1, &normalVBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO[0]);

	//EBO
	glGenBuffers(1, &EBO[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
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
			cout << infoLog << "\n";
			delete[] infoLog;
		}
		return false;
	}
	return true;
}