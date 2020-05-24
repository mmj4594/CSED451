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
    void setRadius(float radius);
    void setSectorCount(int sectorCount);
    void setStackCount(int stackCount);

    void buildVertices();

    void addVertex(float x, float y, float z);
    void addNormal(float x, float y, float z);
    void addTexCoord(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    //void buildInterleavedVertices();
    const float* getInterleavedVertices() const { return interleavedVertices.data(); }
    void draw();
    void clearArrays();

 private:
    float radius;
    int sectorCount, stackCount;
    //std::vector<glm::vec3> vertices;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    //std::vector<glm::uvec3> indices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;

    // interleaved
    std::vector<float> interleavedVertices;
    int interleavedStride;
};