#include <fstream>
#include <iostream>
#include "shaderinfo.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

//Initiate shader program
void initShader() {
	//Adding vertex, fragment shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar vShaderFile[] = "vShader.glvs";
	GLchar fShaderFile[] = "fShader.glfs";
	readShaderSource(vShaderFile, fShaderFile);

	//Link shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	if (!CheckProgram(shaderProgram))
		{ cout << " Link Fail!\n"; }

	//Get location of variables
	aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
	aColorLocation = glGetAttribLocation(shaderProgram, "aColor");
	aNormalLocation = glGetAttribLocation(shaderProgram, "aNormal");
	aTexCoordLocation = glGetAttribLocation(shaderProgram, "aTexCoord");

	lightTypeLocation = glGetUniformLocation(shaderProgram, "lightType");
	ambientProductLocation = glGetUniformLocation(shaderProgram, "ambientProduct");
	diffuseProductLocation_point = glGetUniformLocation(shaderProgram, "diffuseProduct_point");
	diffuseProductLocation_directional = glGetUniformLocation(shaderProgram, "diffuseProduct_directional");
	specularProductLocation_point = glGetUniformLocation(shaderProgram, "specularProduct_point");
	specularProductLocation_directional = glGetUniformLocation(shaderProgram, "specularProduct_directional");

	modelLocation = glGetUniformLocation(shaderProgram, "model");
	viewLocation = glGetUniformLocation(shaderProgram, "view");
	projectionLocation = glGetUniformLocation(shaderProgram, "projection");

	lightPositionLocation_point = glGetUniformLocation(shaderProgram, "lightPosition_point");
	lightPositionLocation_directional = glGetUniformLocation(shaderProgram, "lightPosition_directional");
	shininessLocation = glGetUniformLocation(shaderProgram, "shininess");
	shaderCodeLocation = glGetUniformLocation(shaderProgram, "shaderCode");

	enableTextureLocation = glGetUniformLocation(shaderProgram, "enableTexture");
	mappingCodeLocation = glGetUniformLocation(shaderProgram, "mappingCode");
	textureLocation = glGetUniformLocation(shaderProgram, "texture");
	glUniform1i(textureLocation, 0);
	normalMapLocation = glGetUniformLocation(shaderProgram, "normalMap");
	glUniform1i(normalMapLocation, 1);
	
	//textureLocation = glGetUniformLocation(shaderProgram, "texture");

	glUseProgram(shaderProgram);
	glUniform1i(enableTextureLocation, enableTexture);
	

	//shader는 program 객체와 연결되면 필요x
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*
		VAO를 바인드 한 이후에 바인드되는 VBO, EBO들은 VAO에 저장된다.
		그리고 glBindVertexArray(VAO) 만 수행하면
		해당 VAO에 저장된 VBO 및 EBO가 함께 불러와진다.
	*/
	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//VBO
	glGenBuffers(1, &positionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glGenBuffers(1, &normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glGenBuffers(1, &texCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);

	genTexture();
	genNormalMap();

	

	//EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

//Read shader file from 'vShaderFile', 'fShaderFile' and compile them.
void readShaderSource(const char* vShaderFile, const char* fShaderFile) {
	ifstream v(vShaderFile);
	ifstream f(fShaderFile);
	if (!v.is_open() || !f.is_open()) { cout << "Shader file open Fail\n"; return; }

	string vRawString = string(istreambuf_iterator<char>(v), istreambuf_iterator<char>());
	string fRawString = string(istreambuf_iterator<char>(f), istreambuf_iterator<char>());
	const char* vSource = vRawString.c_str();
	const char* fSource = fRawString.c_str();
	glShaderSource(vertexShader, 1, &vSource, NULL);
	glShaderSource(fragmentShader, 1, &fSource, NULL);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
}

//Check whether shader program works well
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
			cout << infoLog << "\n";
			delete[] infoLog;
		}
		return false;
	}
	return true;
}

void switchShader(const int shaderCode) {
	currentShaderType = shaderCode;
	glUniform1i(shaderCodeLocation, shaderCode);
	if (shaderCode == GOURAUD)
		cout << "\nGOURAUD Shading mode\n\n";
	else if (shaderCode == PHONG) 
		cout << "\nPhong Shading mode\n\n";
}

void genTexture() {
	wall_data = stbi_load("texture/wall_base.jpg", &wall_width, &wall_height, &wall_nrChannels, 0);
	character_data = stbi_load("texture/character.jpg", &character_width, &character_height, &character_nrChannels, 0);
	floor_data = stbi_load("texture/floor.jpg", &floor_width, &floor_height, &floor_nrChannels, 0);

	//Wall Texture
	glGenTextures(1, &wall_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wall_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (wall_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wall_width, wall_height, 0, GL_RGB, GL_UNSIGNED_BYTE, wall_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(wall_data);

	//Character Texture
	glGenTextures(1, &character_texture);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, character_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (character_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, character_width, character_height, 0, GL_RGB, GL_UNSIGNED_BYTE, character_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(character_data);

	//Floor Texture
	glGenTextures(1, &floor_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floor_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (floor_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, floor_width, floor_height, 0, GL_RGB, GL_UNSIGNED_BYTE, floor_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(floor_data);
}

void genNormalMap() {
	wall_normal_data = stbi_load("texture/wall_normal.jpg", &wall_width_normal, &wall_height_normal, &wall_nrChannels_normal, 0);
	character_normal_data = stbi_load("texture/character_normal.png", &character_width_normal, &character_height_normal, &character_nrChannels_normal, 0);
	floor_normal_data = stbi_load("texture/floor_normal.png", &floor_width_normal, &floor_height_normal, &floor_nrChannels_normal, 0);

	//Wall Normal Map
	glGenTextures(1, &wall_normal);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, wall_normal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (wall_normal_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wall_width_normal, wall_height_normal, 0, GL_RGB, GL_UNSIGNED_BYTE, wall_normal_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load normal map" << std::endl;
	}
	stbi_image_free(wall_normal_data);

	//Character Normal Map
	glGenTextures(1, &character_normal);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, character_normal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (character_normal_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, character_width_normal, character_height_normal, 0, GL_RGB, GL_UNSIGNED_BYTE, character_normal_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load normal map" << std::endl;
	}
	stbi_image_free(character_normal_data);

	//Floor Normal Map
	glGenTextures(1, &floor_normal);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, floor_normal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (floor_normal_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, floor_width_normal, floor_height_normal, 0, GL_RGB, GL_UNSIGNED_BYTE, floor_normal_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load normal map" << std::endl;
	}
	stbi_image_free(floor_normal_data);
}