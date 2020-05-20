#pragma once
#include "Wall.h"
#include "character.h"
#include "camera.h"

using namespace std;

//Game Status
const int IDLE = 0;
const int LOSE = 1;
const int WIN = 2;
const int PASS = 3;
const int JUMP = 4;
const int FAIL = 5;

//Size of displayed world
float WORLD_SIZE_X = 200;
float WORLD_SIZE_Y = 100;
//Window size
float WINDOW_WIDTH = 750;
float WINDOW_HEIGHT = 500;

//Wall moving speed
float wallSpeed = 2.5;
float wallHeight = 27.5;
const float wallSpeedIncrement = 0.5;

//Initial position of Player
const float PLAYER_DEFAULT_X = 25.0;
const float PLAYER_DEFAULT_Y = 37.0;

//Initial position of Thief
const float THIEF_DEFAULT_X = 100;
const float THIEF_DEFAULT_Y = 37.0;

//Thief color changing period
int thiefFrame = 50;
int posePeriod = 75;

//Variables of thief jump
bool askJump = false;
bool thiefJumped = false;
float thiefJumpProbability = 0.25;
float jumpCriteria = 20.0;

//Pass, Fail, Jump checker of player
bool isPassed = false;
bool isFailed = false;
bool isJumped = false;

//Lower body animation loop period frame for characters
int animationFrame = 0;
int lowerBodyPeriod = 60;

//Camera zoom in/out size
const float LEFT_INCREMENT = 2;
const float RIGHT_INCREMENT = -7;
const float BOTTOM_INCREMENT = 1.5;
const float TOP_INCREMENT = -4.5;
const float LEFT_DECREMENT = -9;
const float RIGHT_DECREMENT = 0;
const float BOTTOM_DECREMENT = -1.5;
const float TOP_DECREMENT = 4.5;
const float zoomFrame = 50;

//Displacement of life on window
int life = 4;
std::string lifeText = "Life: ";
const float LIFE_DEFAULT_X = 50;
const float LIFE_DEFAULT_Y = 450;
float lifeX = LIFE_DEFAULT_X;
float lifeY = LIFE_DEFAULT_Y;

//Basic objects in world
Wall wall(WORLD_SIZE_X, 20, 0, 10, wallHeight, 0);
character player(PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y, initialPose);
character thief(THIEF_DEFAULT_X, THIEF_DEFAULT_Y, initialPose);

//Camera configuration
int cameraMode = 3;		//point of view
GLdouble eye[3], reference[3], upVector[3];
camera FPV(-WORLD_SIZE_X / 2, WORLD_SIZE_Y / 2 + 20, 0,
	WORLD_SIZE_X / 2, WORLD_SIZE_Y / 4 + 20, 0,
	0, 1, 0);
camera TPV(-WORLD_SIZE_X / 2, WORLD_SIZE_Y / 2 + 20, 180 / 2,
	WORLD_SIZE_X / 2, WORLD_SIZE_Y / 4 + 20, 0 ,
	0, 1, 0 );
camera XYPlane(50, 50, 150,	50, 50, 0, 0, 1, 0);
float fovy = 45;
float newFovy = fovy;
float fovyPerFrame = 0;

//Vertex and fragment shader
GLuint vertexShader;
GLuint fragmentShader;

GLuint shaderProgram;
unsigned int positionVBO, colorVBO, VAO, EBO;
float vertices[] = {
	//position			//color
    /* front surface is blue */
    -20,  20, 20,    0.0, 0.0, 1.0,
    -20, -20, 20,    0.0, 0.0, 1.0,
    20, -20, 20,     0.0, 0.0, 1.0,
    20,  20, 20,     0.0, 0.0, 1.0,
    /* left surface is green */
    -20,  20,  20,   0.0, 1.0, 0.0,
    -20,  20, -20,   0.0, 1.0, 0.0,
    -20, -20, -20,   0.0, 1.0, 0.0,
    -20, -20,  20,   0.0, 1.0, 0.0,
    /* top surface is red */
    -20, 20, 20,     1.0, 0.0, 0.0,
    20, 20, 20,      1.0, 0.0, 0.0,
    20, 20, -20,     1.0, 0.0, 0.0,
    -20, 20, -20,    1.0, 0.0, 0.0,
    /* right surface is yellow */
    20,  20, -20,    1.0, 1.0, 0.0,
    20,  20,  20,    1.0, 1.0, 0.0,
    20, -20,  20,    1.0, 1.0, 0.0,
    20, -20, -20,    1.0, 1.0, 0.0,
    /* back surface is cyan */
    -20,  20, -20,   0.0, 1.0, 1.0,
    20,  20, -20,    0.0, 1.0, 1.0,
    20, -20, -20,    0.0, 1.0, 1.0,
    -20, -20, -20,   0.0, 1.0, 1.0,
    /* bottom surface is magenta */
    -20, -20, -20,   1.0, 0.0, 1.0,
    -20, -20,  20,   1.0, 0.0, 1.0,
    20, -20,  20,    1.0, 0.0, 1.0,
    20, -20, -20,    1.0, 0.0, 1.0
};
unsigned int indices[] = {
     3,1,2,
     3,0,1,

     4,6,7,
     4,5,6,

     8,9,10,
     8,10,11,

     12,13,14,
     12,14,15,

     16,17,19,
     17,18,19,

     20,23,21,
     21,23,22
};
					
//Function definition
void init();
void readShaderSource(const char* vShaderFile, const char* fShaderFile);
bool CheckProgram(GLuint program);
void display3D();
void reshape3D(int w, int h);
void frameAction(int value);
int moveWall();

void keyboard(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);

void writeLife(float x, float y);
void drawFloor();

void setCameraMode(int view) { cameraMode = view; }
void setCamera(camera cameraPos);

void finishGame();

