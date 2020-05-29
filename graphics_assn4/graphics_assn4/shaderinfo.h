#pragma once
#include <GL/glew.h>

//Shader type code
const int GOURAUD = 0, PHONG = 1;
int currentShaderType = 0;

//Vertex and fragment shader
GLuint vertexShader, fragmentShader;
GLuint shaderProgram[2];
unsigned int positionVBO[2], colorVBO[2], normalVBO[2];
unsigned int VAO[2], EBO[2];
int aPosLocation, aColorLocation, aNormalLocation;
int ambientProductLocation, diffuseProductLocation, specularProductLocation;
int modelViewLocation, projectionLocation;
int lightPositionLocation;
int shininessLocation;

void initShader(const int shaderCode);
void readShaderSource(const char* vShaderFile, const char* fShaderFile);
bool CheckProgram(GLuint program);
void switchShader(const int shaderCode);