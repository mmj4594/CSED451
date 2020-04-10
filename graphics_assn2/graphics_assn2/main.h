#pragma once

#include "rect.h"
#include "character.h"
#include "coordinates.h"

//Game Status
const int PLAYING = 0;
const int LOSE = 1;
const int WIN = 2;

//Size of displayed world
float WORLD_X = 150;
float WORLD_Y = 100;

//Wall moving speed
float wallSpeed = 1;
const float wallSpeedIncrement = 0.5;

//Position of Player
const int PLAYER_X = 25;
const int PLAYER_Y = 25;
float playerNewX = PLAYER_X;
//Position of Thief
const int THIEF_X = 75;
const int THIEF_Y = 25;
//Thief color changing period
int thiefFrame = 0;
int colorPeriod = 75;

//Collision Check
bool wallThiefCollision = false;
bool wallPlayerCollsion = false;
//Player moving distance if passes
float movingDistance = 5;
//Camera zoom size if passes
const float LEFT_INCREMENT = 4;
const float RIGHT_INCREMENT = -5;
const float BOTTOM_INCREMENT = 1.5;
const float TOP_INCREMENT = -4.5;
const float zoomFrame = 50;
//World coordinates to implement zoom
coordinates world(0, WORLD_X, 0, WORLD_Y);
coordinates newWorld(0, WORLD_X, 0, WORLD_Y);
coordinates coordinatesIncrement(LEFT_INCREMENT, RIGHT_INCREMENT, BOTTOM_INCREMENT, TOP_INCREMENT);
coordinates incrementPerFrame(LEFT_INCREMENT / zoomFrame, RIGHT_INCREMENT / zoomFrame, BOTTOM_INCREMENT / zoomFrame, TOP_INCREMENT / zoomFrame);

//Displacement of life on window
int life = 4;
std::string lifeText = "Life: ";
const float LIFE_X = 10;
const float LIFE_Y = 90;
float lifeX = LIFE_X;
float lifeY = LIFE_Y;
int frameMoved = 0;

//Basic objects in world
rect world_floor(0, 0, 150, 20);
rect wall(WORLD_X, 20, 10, 50);
character player(PLAYER_X, PLAYER_Y);
character thief(THIEF_X, THIEF_Y);


/*
	Function definition
*/
void init();
void display();
void reshape(int w, int h);
void frameAction(int value);
int moveWall();

void writeLife(float x, float y);

void keyboard(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);

void zoominCamera();

void finishGame();
