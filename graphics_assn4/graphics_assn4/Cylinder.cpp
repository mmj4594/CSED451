#include <iostream>
#include "Cylinder.h"
#include "shaderinfo.h"

using namespace std;

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 1;

Cylinder::Cylinder(float baseRadius, float topRadius, float height, int sectors, int stacks) {
    set(baseRadius, topRadius, height, sectors, stacks);
}

void Cylinder::set(float baseRadius, float topRadius, float height, int sectors, int stacks) {
    this->baseRadius = baseRadius;
    this->topRadius = topRadius;
    this->height = height;
    this->sectorCount = sectors;
    if (sectors < MIN_SECTOR_COUNT)
        this->sectorCount = MIN_SECTOR_COUNT;
    this->stackCount = stacks;
    if (stacks < MIN_STACK_COUNT)
        this->stackCount = MIN_STACK_COUNT;

    // generate unit circle vertices first
    buildUnitCircleVertices();
    buildVertices();
}


void Cylinder::buildUnitCircleVertices() {
    const float PI = acos(-1);
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    std::vector<float>().swap(unitCircleVertices);
    for (int i = 0; i <= sectorCount; ++i) {
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }
}

void Cylinder::buildVertices() {
    // tmp vertex definition (x,y,z,s,t)
    struct Vertex
    {
        float x, y, z, s, t;
    };
    std::vector<Vertex> tmpVertices;

    int i, j, k;    // indices
    float x, y, z, s, t, radius;

    // put tmp vertices of cylinder side to array by scaling unit circle
    //NOTE: start and end vertex positions are same, but texcoords are different
    //      so, add additional vertex at the end point
    for (i = 0; i <= stackCount; ++i)
    {
        z = -(height * 0.5f) + (float)i / stackCount * height;      // vertex position z
        radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);     // lerp
        t = 1.0f - (float)i / stackCount;   // top-to-bottom

        for (j = 0, k = 0; j <= sectorCount; ++j, k += 3)
        {
            x = unitCircleVertices[k];
            y = unitCircleVertices[k + 1];
            s = (float)j / sectorCount;

            Vertex vertex;
            vertex.x = x * radius;
            vertex.y = y * radius;
            vertex.z = z;
            vertex.s = s;
            vertex.t = t;
            tmpVertices.push_back(vertex);
        }
    }

    // clear memory of prev arrays
    clearArrays();

    Vertex v1, v2, v3, v4;      // 4 vertex positions v1, v2, v3, v4
    std::vector<float> n;       // 1 face normal
    int vi1, vi2;               // indices
    int index = 0;

    // v2-v4 <== stack at i+1
    // | \ |
    // v1-v3 <== stack at i
    for (i = 0; i < stackCount; ++i)
    {
        vi1 = i * (sectorCount + 1);            // index of tmpVertices
        vi2 = (i + 1) * (sectorCount + 1);

        for (j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
        {
            v1 = tmpVertices[vi1];
            v2 = tmpVertices[vi2];
            v3 = tmpVertices[vi1 + 1];
            v4 = tmpVertices[vi2 + 1];

            // compute a face normal of v1-v3-v2
            n = computeFaceNormal(v1.x, v1.y, v1.z, v3.x, v3.y, v3.z, v2.x, v2.y, v2.z);

            // put quad vertices: v1-v2-v3-v4
            addVertex(v1.x, v1.y, v1.z);
            addVertex(v2.x, v2.y, v2.z);
            addVertex(v3.x, v3.y, v3.z);
            addVertex(v4.x, v4.y, v4.z);

            // put normal
            for (k = 0; k < 4; ++k)  // same normals for all 4 vertices
            {
                addNormal(n[0], n[1], n[2]);
            }

            // put indices of a quad
            addIndices(index, index + 2, index + 1);    // v1-v3-v2
            addIndices(index + 1, index + 2, index + 3);    // v2-v3-v4

            index += 4;     // for next
        }
    }

    // remember where the base index starts
    baseIndex = (unsigned int)indices.size();
    unsigned int baseVertexIndex = (unsigned int)vertices.size() / 3;

    // put vertices of base of cylinder
    z = -height * 0.5f;
    addVertex(0, 0, z);
    addNormal(0, 0, -1);
    for (i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j + 1];
        addVertex(x * baseRadius, y * baseRadius, z);
        addNormal(0, 0, -1);
    }

    // put indices for base
    for (i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < sectorCount - 1)
            addIndices(baseVertexIndex, k + 1, k);
        else
            addIndices(baseVertexIndex, baseVertexIndex + 1, k);
    }

    // remember where the top index starts
    topIndex = (unsigned int)indices.size();
    unsigned int topVertexIndex = (unsigned int)vertices.size() / 3;

    // put vertices of top of cylinder
    z = height * 0.5f;
    addVertex(0, 0, z);
    addNormal(0, 0, 1);
    for (i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j + 1];
        addVertex(x * topRadius, y * topRadius, z);
        addNormal(0, 0, 1);
    }

    for (i = 0, k = topVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < sectorCount - 1)
            addIndices(topVertexIndex, k, k + 1);
        else
            addIndices(topVertexIndex, k, topVertexIndex + 1);
    }

    // generate interleaved vertex array as well
    initializeColor();
}

void Cylinder::addVertex(float x, float y, float z) {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}
void Cylinder::addNormal(float x, float y, float z) {
    normals.push_back(x);
    normals.push_back(y);
    normals.push_back(z);
}
void Cylinder::addIndices(unsigned int i1, unsigned int i2, unsigned int i3) {
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

void Cylinder::draw() {
    //position VBO    
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        &vertices[0],   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage
    glEnableVertexAttribArray(aPosLocation);
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    //color VBO
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);           // for vertex data
    switch (color) {
    case 0:
        glBufferData(GL_ARRAY_BUFFER,                   // target
            (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
            &vertices_red[0],   // ptr to vertex data
            GL_STATIC_DRAW);    //usage
        break;
    case 1:
        glBufferData(GL_ARRAY_BUFFER,                   // target
            (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
            &vertices_green[0],   // ptr to vertex data
            GL_STATIC_DRAW);    //usage
        break;
    case 2:
        glBufferData(GL_ARRAY_BUFFER,                   // target
            (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
            &vertices_blue[0],   // ptr to vertex data
            GL_STATIC_DRAW);    //usage
        break;
    case 3:
        glBufferData(GL_ARRAY_BUFFER,                   // target
            (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
            &vertices_yellow[0],   // ptr to vertex data
            GL_STATIC_DRAW);    //usage
        break;
    case 5:
        glBufferData(GL_ARRAY_BUFFER,                   // target
            (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
            &vertices_gray[0],   // ptr to vertex data
            GL_STATIC_DRAW);    //usage
        break;
    }
    glEnableVertexAttribArray(aColorLocation);
    glVertexAttribPointer(aColorLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    //Normal VBO
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)normals.size() * sizeof(float), // data size, # of bytes
        &normals[0],   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage
    glEnableVertexAttribArray(aNormalLocation);
    glVertexAttribPointer(aNormalLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    //EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        indices.size() * sizeof(unsigned int),             // data size, # of bytes
        &indices[0],               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // draw a Cylinder with VBO
    glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, glm::value_ptr(mtxView));
    glDrawElements(GL_TRIANGLES,                    // primitive type
        indices.size(),          // # of indices
        GL_UNSIGNED_INT,                 // data type
        0);                       // offset to indices
}

void Cylinder::clearArrays() {
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<unsigned int>().swap(indices);
}

void Cylinder::initializeColor() {
    for (int i = 0; i < vertices.size()/3; i++) {
        //gray
        vertices_gray.push_back(0.3f);
        vertices_gray.push_back(0.3f);
        vertices_gray.push_back(0.3f);

        //red
        vertices_red.push_back(0.8f);
        vertices_red.push_back(0);
        vertices_red.push_back(0);

        //green
        vertices_green.push_back(0);
        vertices_green.push_back(0.8f);
        vertices_green.push_back(0);

        //yellow
        vertices_yellow.push_back(0.9f);
        vertices_yellow.push_back(0.9f);
        vertices_yellow.push_back(0);

        //blue
        vertices_blue.push_back(0);
        vertices_blue.push_back(0);
        vertices_blue.push_back(0.8f);
    }
}

std::vector<float> Cylinder::computeFaceNormal(float x1, float y1, float z1,  // v1
    float x2, float y2, float z2,  // v2
    float x3, float y3, float z3)  // v3
{
    const float EPSILON = 0.000001f;

    std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
    float nx, ny, nz;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;

    // cross product: e1 x e2
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if (length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;
    }

    return normal;
}
