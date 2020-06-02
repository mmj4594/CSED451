#include <GL/glew.h>
#include <iostream>
#include <time.h>

#include "main.h"
#include "colors.h"
#include "shaderinfo.h"
#include "light.h"

//All pass/fail cheat activation status
static bool allPass = false;
static bool allFail = false;

//Game status
int gameStatus = IDLE;

using namespace std;
using namespace glm;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("assn4");
	glutDisplayFunc(display3D);
	glutReshapeFunc(reshape3D);
	glutTimerFunc(0, frameAction, 1);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);

	init();
	glutMainLoop();

	//For safe unbound of VBO, VAO, EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);			//VBO
	glBindVertexArray(0);						//VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	//EBO

	//deallocate all resources when program ends
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &positionVBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram[0]);
	glDeleteProgram(shaderProgram[1]);
	return 0;
}

void init() {
	srand(time(NULL));
	glewInit();
	writeLife();
	
	//Init shaders
	initShader(GOURAUD);
	initShader(PHONG);
	switchShader(GOURAUD);

	//Initial Camera Setting
	setCamera(TPV);

	//Backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//Hidden surface removal
	glEnable(GL_DEPTH_TEST);
}

//Display current screen
void display3D() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*	Wireframe mode or Filling mode	*/
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	

	//projection
	mtxProj = perspective(radians(fovy), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 2000.0f);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(mtxProj));
	//modelView
	mtxView = lookAt(
		vec3(eye[0], eye[1], eye[2]),
		vec3(reference[0], reference[1], reference[2]),
		vec3(upVector[0], upVector[1], upVector[2])
	);

	//Ambient
	glUniform4fv(ambientProductLocation, 1, value_ptr(ambient));
	//Diffuse
	glUniform4fv(diffuseProductLocation_point, 1, value_ptr(diffuse_point));
	glUniform4fv(diffuseProductLocation_directional, 1, value_ptr(diffuse_directional));
	//Specular
	glUniform4fv(specularProductLocation_point, 1, value_ptr(specular_point));
	glUniform4fv(specularProductLocation_directional, 1, value_ptr(specular_directional));
	//Light poisition
	glUniform4fv(lightPositionLocation_point, 1, value_ptr(lightPosition_point));
	glUniform4fv(lightPositionLocation_directional, 1, value_ptr(lightPosition_directional)); 
	//Shininess
	glUniform1f(shininessLocation, shininess);
	
	worldFloor.draw();
	wall.draw();
	player.draw();
	thief.draw();
	glutSwapBuffers();
}

//Action when window of the game reshaped(3D)
void reshape3D(int w, int h) {
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;
	glClearColor(1, 1, 1, 1);
	glViewport(0, 0, w, h);
	mtxProj = glm::perspective(glm::radians(fovy), (float)w / h, 1.0f, 2000.0f);
	glutPostRedisplay();
}

//Action per frame(60FPS currently)
void frameAction(int value) {
	//Change pose of the thief in every set period
	if (thiefFrame >= posePeriod) {
		switch (rand() % 4) {
		case 0: thief.changePose(poseA); break;
		case 1: thief.changePose(poseB); break;
		case 2: thief.changePose(poseC); break;
		case 3: thief.changePose(poseD); break;
		}
		thiefFrame = 0;
	}

	//move wall and get current status.
	//gameStatus = moveWall();
	switch (gameStatus) {
	case LOSE: cout << "Lose\n"; finishGame(); return;
	case WIN: cout << "Win\n"; finishGame(); return;
	case PASS: isPassed = true; break;
	case JUMP: isJumped = true; break;
	case FAIL: isFailed = true; break;
	}

	/*
		Move player to right and zoom camera if pass successfully
		after the wall disappear and player land successfully
	*/
	if ((wall.getX() + wall.getWidth() < 0) && isPassed && (player.getY() == PLAYER_DEFAULT_Y)) {
		wallSpeed += wallSpeedIncrement;
		posePeriod -= 5;
		player.setnewX(player.getX() + player.getMovingDistance());

		newFovy = fovy * 0.85;
		fovyPerFrame = (newFovy - fovy) / zoomFrame;
		isPassed = false;
	}
	/*
		Zoom out camera if thief doesn't jump and player ignore the wall by jump
		after the wall disappear and player land successfully
	*/
	else if ((wall.getX() + wall.getWidth() < 0) && isJumped && wall.getColor() != GRAY && (player.getY() == PLAYER_DEFAULT_Y)) {
		newFovy = fovy * 1.05;
		fovyPerFrame = (newFovy - fovy) / zoomFrame;
		isJumped = false;
	}

	//Camera manipulation
	FPV = camera(player.getX() + 3, player.getY() + 8, 0,
		thief.getX(), player.getY() + 8, 0,
		0, 1, 0);
	camera7 = camera(lightPosition_directional.x, lightPosition_directional.y, lightPosition_directional.z,
		0, 0, 0,
		0, 1, 0);
	switch (cameraMode) {
	case 1:	setCamera(FPV); break;
	case 2: setCamera(camera2); break;
	case 3:	setCamera(TPV);	break;
	case 4: setCamera(camera4); break;
	case 5: setCamera(camera5); break;
	case 6: setCamera(camera6); break;
	case 7: setCamera(camera7); break;
	case 8: setCamera(camera8); break;
	case 9: setCamera(XYPlane); break;
	}
	if (abs(newFovy - fovy) > 0.01) { fovy += fovyPerFrame; }
	//Light position manipulation
	lightPosition_point = glm::vec4(wall.getX(), wall.getY() + wall.getHeight() + 5, wall.getZ(), 1.0);
	lightPosition_directional = rotate(mat4(1.0f), radians(lightAngle_directional), vec3(0, 0, 1)) *
					vec4(lightCenter_directional.x + 200 * cos(radians(-90.0f + 0.5*lightFrame)),
						 lightCenter_directional.y + 0,
						 lightCenter_directional.z + 200 * sin(radians(-90.0f + 0.5*lightFrame)), 0.0);
	if (lightFrame >= SEC * 6) {
		/*if (diffuse_directional == DARK) { diffuse_directional = BRIGHT; specular_directional = BRIGHT; }
		else { diffuse_directional = DARK; specular_directional = DARK; }*/
		lightFrame = 0;
	}

	//Ask if thief will jump
	if ((wall.getX() - thief.getX() < 35) && !askJump) {
		askJump = true;
		//thief jump with probability
		if (rand() % 100 < thiefJumpProbability * 100) {
			thief.jump();
			thiefJumped = true;
		}
	}
	//Change pose of the thief in every set period
	if (thiefFrame >= posePeriod) {
		switch (rand() % 4) {
		case 0: thief.changePose(poseA); break;
		case 1: thief.changePose(poseB); break;
		case 2: thief.changePose(poseC); break;
		case 3: thief.changePose(poseD); break;
		}
		thiefFrame = 0;
	}

	//Check new position of characters
	player.checkNewPosition();
	thief.checkNewPosition();

	//Animation loop for player and thief
	thiefFrame++; animationFrame++; lightFrame++;
	if (animationFrame >= SEC) { animationFrame = 0; }
	player.lowerBodyAnimation(animationFrame, SEC);
	thief.lowerBodyAnimation(animationFrame, SEC);
	player.upperBodyAnimation();
	thief.upperBodyAnimation();

	glutPostRedisplay();
	glutTimerFunc(17, frameAction, 1);		//call timer function recursively until game ends
}

/*
	Move wall in 'wallSpeed'.
	And if there is collision between wall and other object, return the game status
	according to the type of object where the collision occurs.
*/
int moveWall() {
	int status = IDLE;

	wall.setX(wall.getX() - 0.3 * wallSpeed);

	//Collision between wall and player
	if (wall.collisionCheck(&player)) {
		//Jump
		if (!allFail && !allPass && player.getY() > PLAYER_DEFAULT_Y + jumpCriteria) {
			status = JUMP;
		}
		//Fail
		else if (!allPass && (allFail || wall.getColor() != player.getColor() || (wall.getColor() == GRAY && player.getColor() == GRAY))) {
			status = FAIL;
			cout << "Fail\n";
			
			life--;
			writeLife();
			//Lose
			if (life <= 0) status = LOSE;
		}
		//Pass
		else if (!allFail && (allPass || wall.getColor() == player.getColor())) {
			status = PASS;
			cout << "Pass\n";
		}
	}
	//Collision between wall and thief
	else if (wall.collisionCheck(&thief)) {
		if (!thiefJumped) {
			//Set shape and color of wall
			wall.setColor(thief.getColor());
			wall.setShape(thief.getColor());
		}
	}
	//Collision between player and thief
	else if (player.collisionCheck(&thief))
		status = WIN;

	//Repositioning of wall when it goes out of the screen
	if (wall.getX() + wall.getWidth() + wallSpeed * 20 < 0) {
		int shape = wall.getShape();
		wall = Wall(WORLD_SIZE_X, 0, 0, 10, wallHeight, 0);
		wall.setShape(shape);
		thief.resetCollided();
		player.resetCollided();
		askJump = false;
		thiefJumped = false;
		isPassed = false;
		isJumped = false;
		isFailed = false;
		wall.setShape(5);
	}
	return status;
}


//Define cheat according to user keyboard input.
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'c':
		allFail = false;
		allPass = true;
		cout << "All pass\n";
		break;
	case 'f':
		allPass = false;
		allFail = true;
		cout << "All fail\n";
		break;
	case 'q':
		if (currentShaderType == GOURAUD) switchShader(PHONG);
		else if (currentShaderType == PHONG) switchShader(GOURAUD);
		break;
	case SPACE:
		player.jump();
		break;
	case '1': if (cameraMode != 1) { fovy *= 2; newFovy *= 2; } setCameraMode(1); break;
	case '2': if (cameraMode == 1) { fovy /= 2; newFovy /= 2; } setCameraMode(2); break;
	case '3': if (cameraMode == 1) { fovy /= 2; newFovy /= 2; }	setCameraMode(3); break;
	case '4': if (cameraMode == 1) { fovy /= 2; newFovy /= 2; } setCameraMode(4); break;
	case '5': if (cameraMode == 1) { fovy /= 2; newFovy /= 2; } setCameraMode(5); break;
	case '6': if (cameraMode == 1) { fovy /= 2; newFovy /= 2; } setCameraMode(6); break;
	case '7': if (cameraMode == 1) { fovy /= 2; newFovy /= 2; } setCameraMode(7); break;
	case '8': if (cameraMode == 1) { fovy /= 2; newFovy /= 2; } setCameraMode(8); break;
	case '9': if (cameraMode == 1) { fovy /= 2; newFovy /= 2; } setCameraMode(9); break;
	}

	//Animation loop for player and thief
	glutPostRedisplay();
}

//Determine color of player according to user keyboard input.
void specialkeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: player.changePose(poseA); break;
	case GLUT_KEY_DOWN: player.changePose(poseB); break;
	case GLUT_KEY_LEFT: player.changePose(poseC); break;
	case GLUT_KEY_RIGHT: player.changePose(poseD); break;
	}
}

//Set camera to cameraPos
void setCamera(camera cameraPos) {
	memcpy(eye, cameraPos.getEye(), sizeof(eye));
	memcpy(reference, cameraPos.getReference(), sizeof(reference));
	memcpy(upVector, cameraPos.getUpVector(), sizeof(upVector));
}

//Fnish game
void finishGame() {
	glutPostRedisplay();
	glutReshapeFunc(NULL);
	glutIdleFunc(NULL);
	glutKeyboardFunc(NULL);
	glutSpecialFunc(NULL);
}

void writeLife() {
	string s = lifeText + to_string(life);
	cout << s << endl;
}