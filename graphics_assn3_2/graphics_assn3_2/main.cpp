#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include "main.h"
#include "colors.h"

using namespace std;

GLuint shaderProgram;
unsigned int VBO, VAO, EBO;
float vertices[] = {
	0.5, 0.5, 0.0,		1.0, 0.0, 0.0,
	0.5, -0.5, 0.0,		0.0, 1.0, 0.0,
	-0.5, -0.5, 0.0,	0.0, 0.0, 1.0
	//-0.5, 0.5, 0.0,		0.5, 0.5, 0.5,
};
unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};
stack<glm::mat4> modelViewStack;
stack<glm::mat4> projectionStack;

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

	//For safe unbound of VBO, VAO, EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);			//VBO
	glBindVertexArray(0);						//VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	//EBO

	//deallocate all resources when program ends
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
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
	GLuint vertexShader;
	GLchar vShaderfile[] = "vShader.glvs";
	const string vRawString = readShaderSource(vShaderfile);
	const char* vSource = vRawString.c_str();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vSource, NULL);
	glCompileShader(vertexShader);

	//Adding fragment shader
	GLuint fragmentShader;
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
	if (!CheckProgram(shaderProgram)) { cout << "Link Fail!\n"; }
	glDeleteShader(vertexShader);		//shader�� program ��ü�� ����Ǹ� �ʿ�x
	glDeleteShader(fragmentShader);


	/*
		VAO�� ���ε� �� ���Ŀ� ���ε�Ǵ� VBO, EBO���� VAO�� ����ȴ�.
		�׸��� display �Լ� ������ glBindVertexArray(VAO) �� �����ϸ�
		�ش� VAO�� ����� VBO �� EBO�� �Բ� �ҷ�������.
	*/
	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Linking Vertex Attributes (��ġ)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Linking Vertex Attributes (�÷�)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	//For safe unbound of VBO, VAO, EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);			//VBO
	glBindVertexArray(0);						//VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	//EBO
}

void display3D() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*	Wireframe mode or Filling mode	*/
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//�츮�� shader program���� 'view'��� �̸��� uniform ������ ��ġ�� ����.
	int m_uniProjection = glGetUniformLocation(shaderProgram, "projection");
	int m_uniView = glGetUniformLocation(shaderProgram, "modelView");

	//projection
	glm::mat4 matProj;
	//matProj = glm::perspective(45.0f, 640.0f / 480.0f, 1.0f, 500.f);
	matProj = glm::ortho(-1, 1, -1, 1);
	glUniformMatrix4fv(m_uniProjection, 1, GL_FALSE, glm::value_ptr(matProj));

	//modelView
	glm::mat4 matView = glm::mat4(1.0f);
	glUniformMatrix4fv(m_uniView, 1, GL_FALSE, glm::value_ptr(matView));
	


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