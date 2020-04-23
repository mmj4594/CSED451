#pragma once

#include "rect.h"
#include "character.h"
#include "coordinates.h"


//Game Status
const int IDLE = 0;
const int LOSE = 1;
const int WIN = 2;
const int PASS = 3;
const int JUMP = 4;
const int FAIL = 5;

//Size of displayed world
float WORLD_SIZE_X = 150;
float WORLD_SIZE_Y = 100;

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
int colorPeriod = 75;

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

//World coordinates to implement zoom
coordinates world(0, WORLD_SIZE_X, 0, WORLD_SIZE_Y);
coordinates newWorld(0, WORLD_SIZE_X, 0, WORLD_SIZE_Y);
coordinates coordinatesIncrement(LEFT_INCREMENT, RIGHT_INCREMENT, BOTTOM_INCREMENT, TOP_INCREMENT);
coordinates incrementPerFrame(LEFT_INCREMENT / zoomFrame, RIGHT_INCREMENT / zoomFrame, BOTTOM_INCREMENT / zoomFrame, TOP_INCREMENT / zoomFrame);
coordinates coordinatesDecrement(LEFT_DECREMENT, RIGHT_DECREMENT, BOTTOM_DECREMENT, TOP_DECREMENT);
coordinates decrementPerFrame(LEFT_DECREMENT / zoomFrame, RIGHT_DECREMENT / zoomFrame, BOTTOM_DECREMENT / zoomFrame, TOP_DECREMENT / zoomFrame);
coordinates coordinatesMoveCameraLeft(-jump_back, -jump_back, 0, 0);
coordinates moveCameraLeftPerFrame(-jump_back / zoomFrame, -jump_back / zoomFrame, 0, 0);


//Displacement of life on window
int life = 4;
std::string lifeText = "Life: ";
const float LIFE_DEFAULT_X = 10;
const float LIFE_DEFAULT_Y = 90;
float lifeX = LIFE_DEFAULT_X;
float lifeY = LIFE_DEFAULT_Y;

//Basic objects in world
rect world_floor(-200, -30, 350, 50);
rect wall(WORLD_SIZE_X, 20, 10, wallHeight);
character player(PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y, initialPose);
character thief(THIEF_DEFAULT_X, THIEF_DEFAULT_Y, initialPose);


//Function definition
void init();
void display();
void reshape(int w, int h);
void frameAction(int value);
int moveWall();

void writeLife(float x, float y);

void keyboard(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);

void moveCameraLeft();
void zoominCamera();
void zoomoutCamera();

void finishGame();



