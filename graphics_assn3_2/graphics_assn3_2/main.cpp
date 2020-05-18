#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

GLuint shaderProgram;
unsigned int VBO, VAO;
float vertices[] = {
	-0.5, -0.5, 0.0,
	0.5, -0.5, 0.0,
	0.0, 0.5, 0.0
};

static char* readShaderSource(const char* shaderFile);
void init();
void display3D();

using namespace std;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(750, 500);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("assn3_2");
	glutDisplayFunc(display3D);
	//glutReshapeFunc(reshape3D);
	//glutTimerFunc(0, frameAction, 1);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(specialkeyboard);

	init();
	glutMainLoop();

	//deallocate all resources when program ends
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	return 0;
}


static char* readShaderSource(const char* shaderFile) {
	FILE* fp = fopen(shaderFile, "r");
	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';
	fclose(fp);
	return buf;
}


void init() {
	GLenum err = glewInit();
	//Adding vertex shader
	GLuint vertexShader;
	GLchar vShaderfile[] = "vShader.glvs";
	GLchar* vSource = readShaderSource(vShaderfile);
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vSource, NULL);
	glCompileShader(vertexShader);

	//Adding fragment shader
	GLuint fragmentShader;
	GLchar fShaderfile[] = "fshader.glfs";
	GLchar* fSource = readShaderSource(fShaderfile);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fSource, NULL);
	glCompileShader(fragmentShader);

	//Shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glUseProgram(shaderProgram);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);		//shader를 program 객체로 연결하면 필요x
	glDeleteShader(fragmentShader);

	//VAO and VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Linking Vertex Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void display3D() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers();
}
