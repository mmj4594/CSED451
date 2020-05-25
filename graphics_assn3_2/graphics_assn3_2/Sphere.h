#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <algorithm>
#include <vector>

#include "matrixStack.h"

class Sphere
{
public:
    // ctor/dtor
    Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18);
    float getRadius() const { return radius; }
    int getSectorCount() const { return sectorCount; }
    int getStackCount() const { return stackCount; }
    void set(float radius, int sectorCount, int stackCount);
    void setColor(int color) { this->color = color; };
    
    void buildVertices();

    void addVertex(float x, float y, float z);

    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);

    void draw();
    void clearArrays();
    void initializeColor();

 private:
    float radius;
    int sectorCount, stackCount;
    int color;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;

    //vertices color
    std::vector<float> vertices_gray;
    std::vector<float> vertices_red;
    std::vector<float> vertices_green;
    std::vector<float> vertices_yellow;
    std::vector<float> vertices_blue;
};