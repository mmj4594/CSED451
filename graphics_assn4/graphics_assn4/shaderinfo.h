#pragma once
#include <GL/glew.h>

//Shader type code
const int GOURAUD = 0, PHONG = 1;
const int NORMAL = 0, DIFFUSE = 1;
int currentShaderType = 0;
bool enableTexture = true;
int mappingCode = 1;

//Vertex and fragment shader
GLuint vertexShader, fragmentShader;
GLuint shaderProgram;
unsigned int positionVBO, colorVBO, normalVBO, texCoordVBO;
unsigned int VAO, EBO;
int aPosLocation, aColorLocation, aNormalLocation, aTexCoordLocation;
int lightTypeLocation;
int ambientProductLocation;
int diffuseProductLocation_point, diffuseProductLocation_directional;
int specularProductLocation_point, specularProductLocation_directional;
int modelLocation, viewLocation, projectionLocation;
int lightPositionLocation_point, lightPositionLocation_directional;
int shininessLocation;
int shaderCodeLocation;
int enableTextureLocation;
int mappingCodeLocation;

unsigned int wall_texture, character_texture, floor_texture;
int textureLocation;
int wall_width, wall_height, wall_nrChannels;
int character_width, character_height, character_nrChannels;
int floor_width, floor_height, floor_nrChannels;

unsigned char* wall_data;
unsigned char* character_data;
unsigned char* floor_data;

void initShader();
void readShaderSource(const char* vShaderFile, const char* fShaderFile);
bool CheckProgram(GLuint program);
void switchShader(const int shaderCode);
void switchTexture(const int textureCode);