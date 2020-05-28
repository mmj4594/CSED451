#pragma once
#include <GL/glew.h>

//Vertex and fragment shader
GLuint vertexShader, fragmentShader;
GLuint shaderProgram[2];	//0 for Gouraud, 1 for Phong
unsigned int positionVBO[2], colorVBO[2], normalVBO[2];
unsigned int VAO[2], EBO[2];
int aPosLocation, aColorLocation, aNormalLocation;
int ambientProductLocation, diffuseProductLocation, specularProductLocation;
int modelViewLocation, projectionLocation;
int lightPositionLocation;
int shininessLocation;

void initShader();
void readShaderSource(const char* vShaderFile, const char* fShaderFile);
bool CheckProgram(GLuint program);