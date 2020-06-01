#pragma once

//Light configuration
int lightFrame = 0;
const glm::vec4 BRIGHT = glm::vec4(0.5, 0.5, 0.5, 1.0);
const glm::vec4 DARK = glm::vec4(0.2, 0.2, 0.2, 1.0);
glm::vec4 ambient = glm::vec4(0.0, 0.0, 0.0, 0.0);
glm::vec4 diffuse_point = glm::vec4(0, 0, 0, 1.0);
glm::vec4 diffuse_directional = glm::vec4(1.0, 1.0, 1.0, 1.0);
glm::vec4 specular_point = glm::vec4(0, 0, 0, 1.0);
glm::vec4 specular_directional = glm::vec4(1.0, 1.0, 1.0, 1.0);
glm::vec4 lightPosition_point = glm::vec4(0, 0, 0, 1.0);
glm::vec4 lightPosition_directional = glm::vec4(0, 0, -100.0, 0.0);
glm::vec3 lightCenter_directional = glm::vec3(0.0, 0.0, 0.0);
float lightAngle_directional = 45.0f;
float shininess = 0.0f;