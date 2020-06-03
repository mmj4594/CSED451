#pragma once
#include <GL/glew.h>

//Shader type code
const int GOURAUD = 0, PHONG = 1;
int currentShaderType = 0;

//Vertex and fragment shader
GLuint vertexShader, fragmentShader;
GLuint shaderProgram;
unsigned int positionVBO, colorVBO, normalVBO;
unsigned int VAO, EBO;
int aPosLocation, aColorLocation, aNormalLocation;
int lightTypeLocation;
int ambientProductLocation;
int diffuseProductLocation_point, diffuseProductLocation_directional;
int specularProductLocation_point, specularProductLocation_directional;
int modelLocation, viewLocation, projectionLocation;
int lightPositionLocation_point, lightPositionLocation_directional;
int shininessLocation;
int shaderCodeLocation;

void initShader();
void readShaderSource(const char* vShaderFile, const char* fShaderFile);
bool CheckProgram(GLuint program);
void switchShader(const int shaderCode);