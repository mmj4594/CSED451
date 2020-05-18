#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;
unsigned int VBO, VAO;
float vertices[] = {
	-0.5, -0.5, 0.0,
	0.5, -0.5, 0.0,
	0.0, 0.5, 0.0
};

const string readShaderSource(const char* shaderFile);
void init();
void display3D();
bool CheckProgram(GLuint program);

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


const string readShaderSource(const char* shaderFile) {
	ifstream f(shaderFile);
	if (!f.is_open()) return NULL;
	return string(istreambuf_iterator<char>(f), istreambuf_iterator<char>());
}


void init() {
	glewInit();
	//Adding vertex shader
	GLchar vShaderfile[] = "vShader.glvs";
	const string vRawString = readShaderSource(vShaderfile);
	const char* vSource = vRawString.c_str();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vSource, NULL);
	glCompileShader(vertexShader);

	//Adding fragment shader
	GLchar fShaderfile[] = "fshader.glfs";
	const string fRawString = readShaderSource(fShaderfile);
	const char* fSource = fRawString.c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fSource, NULL);
	glCompileShader(fragmentShader);

	//Shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	/*if (!CheckProgram(shaderProgram)) {
		cout << "Link Fail!\n";
	}*/
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

bool CheckProgram(GLuint program) {
	GLint state;
	glGetProgramiv(program, GL_LINK_STATUS, &state);
	if (GL_FALSE == state) {
		int infologLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 1) {
			int charsWritten = 0;
			char* infoLog = new char[infologLength];
			glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
			std::cout << infoLog << std::endl;
			delete[] infoLog;
		}
		return false;
	}
	return true;
}