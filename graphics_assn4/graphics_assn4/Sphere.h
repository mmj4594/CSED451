#pragma once

#include <GL/glew.h>
#include <vector>

#include "matrixStack.h"

/*
    \class	Sphere

    \brief
    This class defines sphere object that is used
    to implement head and joint of character.

    \detail
    float radius: Radius of the sphere
    int sectorCount: # of slices
    int stackCount: # of stacks
    int color: current color of sphere
    vector<float> vertices: Vertices that make up the sphere
    vector<unsigned int> indices: Indices of vertices
    vector<float> vertices_gray/red/green/yellow/blue: Color of vertices
*/
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
    std::vector<unsigned int> indices;

    //vertices color
    std::vector<float> vertices_gray;
    std::vector<float> vertices_red;
    std::vector<float> vertices_green;
    std::vector<float> vertices_yellow;
    std::vector<float> vertices_blue;
};