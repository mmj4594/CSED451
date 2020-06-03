#include <fstream>
#include <iostream>
#include "shaderinfo.h"

using namespace std;

//Initiate shader program
void initShader() {
	//Adding vertex, fragment shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar vShaderFile[] = "vShader.glvs";
	GLchar fShaderFile[] = "fShader.glfs";
	readShaderSource(vShaderFile, fShaderFile);

	//Link shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	if (!CheckProgram(shaderProgram))
		{ cout << " Link Fail!\n"; }
	//Get location of variables
	aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
	aColorLocation = glGetAttribLocation(shaderProgram, "aColor");
	aNormalLocation = glGetAttribLocation(shaderProgram, "aNormal");
	lightTypeLocation = glGetUniformLocation(shaderProgram, "lightType");
	ambientProductLocation = glGetUniformLocation(shaderProgram, "ambientProduct");
	diffuseProductLocation_point = glGetUniformLocation(shaderProgram, "diffuseProduct_point");
	diffuseProductLocation_directional = glGetUniformLocation(shaderProgram, "diffuseProduct_directional");
	specularProductLocation_point = glGetUniformLocation(shaderProgram, "specularProduct_point");
	specularProductLocation_directional = glGetUniformLocation(shaderProgram, "specularProduct_directional");
	modelLocation = glGetUniformLocation(shaderProgram, "model");
	viewLocation = glGetUniformLocation(shaderProgram, "view");
	projectionLocation = glGetUniformLocation(shaderProgram, "projection");
	lightPositionLocation_point = glGetUniformLocation(shaderProgram, "lightPosition_point");
	lightPositionLocation_directional = glGetUniformLocation(shaderProgram, "lightPosition_directional");
	shininessLocation = glGetUniformLocation(shaderProgram, "shininess");
	shaderCodeLocation = glGetUniformLocation(shaderProgram, "shaderCode");
	glUseProgram(shaderProgram);
	//shader는 program 객체와 연결되면 필요x
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*
		VAO를 바인드 한 이후에 바인드되는 VBO, EBO들은 VAO에 저장된다.
		그리고 glBindVertexArray(VAO) 만 수행하면
		해당 VAO에 저장된 VBO 및 EBO가 함께 불러와진다.
	*/
	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//VBO
	glGenBuffers(1, &positionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glGenBuffers(1, &normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);

	//EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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

void switchShader(const int shaderCode) {
	currentShaderType = shaderCode;
	glUniform1i(shaderCodeLocation, shaderCode);
	if (shaderCode == GOURAUD)
		cout << "\nGOURAUD Shading mode\n\n";
	else if (shaderCode == PHONG) 
		cout << "\nPhong Shading mode\n\n";
}