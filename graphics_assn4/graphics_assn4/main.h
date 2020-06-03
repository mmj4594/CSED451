#pragma once
#include <string>

#include "Wall.h"
#include "character.h"
#include "camera.h"
#include "Floor.h"

using namespace std;

//Key value for space
#define SPACE 32

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
const float PLAYER_DEFAULT_Y = 17.0;

//Initial position of Thief
const float THIEF_DEFAULT_X = 100;
const float THIEF_DEFAULT_Y = 17.0;

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
const int SEC = 60;
const float zoomFrame = 50;

//Displacement of life on window
int life = 500;
std::string lifeText = "Life: ";
const float LIFE_DEFAULT_X = 50;
const float LIFE_DEFAULT_Y = 450;
float lifeX = LIFE_DEFAULT_X;
float lifeY = LIFE_DEFAULT_Y;

//Basic objects in world
Wall wall(WORLD_SIZE_X, 0, 0, 10, wallHeight, 0);
character player(PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y, initialPose);
character thief(THIEF_DEFAULT_X, THIEF_DEFAULT_Y, initialPose);
GLfloat floorSize = 300;
GLfloat gridSize = 25;
Floor worldFloor(floorSize, gridSize);

//Camera configuration
int cameraMode = 3;		//point of view
GLdouble eye[3], reference[3], upVector[3];
camera FPV(PLAYER_DEFAULT_X + 3, PLAYER_DEFAULT_Y + 5 + 3, 0,
    THIEF_DEFAULT_X, THIEF_DEFAULT_Y + 5 + 3, 0,
    0, 1, 0);
camera camera2(-80, 35, 0, 0, 35, 0, 0, 1, 0);
camera TPV(-WORLD_SIZE_X / 2, WORLD_SIZE_Y / 2, 180 / 2,
    WORLD_SIZE_X / 2, WORLD_SIZE_Y / 4 + 20, 0,
    0, 1, 0);
camera camera4(-WORLD_SIZE_X / 2, WORLD_SIZE_Y / 2, -180 / 2,
    WORLD_SIZE_X / 2, WORLD_SIZE_Y / 4 + 20, 0,
    0, 1, 0);
camera camera5(250, 35, 0, 0, 35, 0, 0, 1, 0);
camera camera6(100, 200, 0, 50, 0, 0, 0, 1, 0);
camera camera7(0, 0, 0, 0, 0, 0, 0, 1, 0);
camera camera8(80, 25, -150, 80, 25, 0, 0, 1, 0);
camera XYPlane(80, 25, 150, 80, 25, 0, 0, 1, 0);
float fovy = 45;
float newFovy = fovy;
float fovyPerFrame = 0;
					
//Function definition
void init();
void readShaderSource(const char* vShaderFile, const char* fShaderFile);
void display3D();
void reshape3D(int w, int h);
void frameAction(int value);
int moveWall();

void keyboard(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);

void writeLife();

void setCameraMode(int view) { cameraMode = view; }
void setCamera(camera cameraPos);

void finishGame();

