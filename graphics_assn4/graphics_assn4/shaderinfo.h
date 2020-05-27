#pragma once
#include <GL/glew.h>

//Vertex and fragment shader
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;
unsigned int positionVBO, colorVBO, VAO, EBO;
int aPosLocation, aColorLocation, lightColorLocation;
int modelViewLocation, projectionLocation;

void initShader();
void readShaderSource(const char* vShaderFile, const char* fShaderFile);
bool CheckProgram(GLuint program);