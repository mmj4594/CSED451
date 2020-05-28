#pragma once
#include <GL/glew.h>
#include <vector>

#include "matrixStack.h"

/*
    \class	Cylinder

    \brief
    This class defines cylinder object that used to implement
    walls and body of character.

    \detail
    float baseRadius: Radius of bottom circle
    float topRadius: Radius of top circle
    float height: height of the cylinder
    int sectorCount: # of slices
    int stackCount: # of stacks
    unsigned int baseIndex: starting index of base
    unsigned int topIndex: starting index of top

    vector<float> unitCircleVertices: Vertices that make up unit circle
    vector<float> vertices: Vertices that make up the cylinder
    vector<unsigned int> indices: Indices of vertices
    vector<float> vertices_gray/red/green/yellow/blue: Color of vertices
*/
class Cylinder
{
public:
    // ctor/dtor
    Cylinder() { baseRadius = topRadius = height = 0; sectorCount = stackCount = baseIndex = topIndex = 0; }
    Cylinder(float baseRadius, float topRadius, float height, int sectors, int stacks);
    float getBaseRadius() const { return baseRadius; }
    float getTopRadius() const { return topRadius; }
    float getHeight() const { return height; }
    int getSectorCount() const { return sectorCount; }
    int getStackCount() const { return stackCount; }
    void set(float baseRadius, float topRadius, float height, int sectorCount, int stackCount);

    void buildVertices();

    void addVertex(float x, float y, float z);
    void addNormal(float x, float y, float z);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    void setColor(int color) { this->color = color; };
    void draw();
    void clearArrays();
    void initializeColor();

private:
    void buildUnitCircleVertices();

    float baseRadius;
    float topRadius;
    float height;
    int sectorCount;                        // # of slices
    int stackCount;                         // # of stacks

    unsigned int baseIndex;                 // starting index of base
    unsigned int topIndex;                  // starting index of top

    std::vector<float> unitCircleVertices;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> normals;

    //vertices color
    std::vector<float> vertices_gray;
    std::vector<float> vertices_red;
    std::vector<float> vertices_green;
    std::vector<float> vertices_yellow;
    std::vector<float> vertices_blue;
    int color = 5;
};