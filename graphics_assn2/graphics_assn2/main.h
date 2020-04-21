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
float wallSpeed = 2.5;
float wallHeight = 27.5;
const float wallSpeedIncrement = 0.5;

//Initial position of Player
const float PLAYER_X = 25.0;
const float PLAYER_Y = 37.0;

//Initial position of Thief
const float THIEF_X = 100;
const float THIEF_Y = 37.0;

//Thief color changing period
int thiefFrame = 50;
int colorPeriod = 75;

//Variables of thief jump
bool askJump = false;
bool thiefJumped = false;
float thiefJumpProbability = 0.25;
float jumpCriteria = 20.0;

//Check if player pass the wall
bool pass = false;

//Lower body animation loop period frame for characters
int animationFrame = 0;
int lowerBodyPeriod = 60;

//Camera zoom size if passes
const float LEFT_INCREMENT = 0;
const float RIGHT_INCREMENT = -9;
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
rect wall(WORLD_X, 20, 10, wallHeight);
character player(PLAYER_X, PLAYER_Y, poseA);
character thief(THIEF_X, THIEF_Y, poseA);


//Function definition
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



