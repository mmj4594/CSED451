#include "Sphere.h"
#include "shaderinfo.h"
#include <math.h>
#include <iostream>

using namespace std;

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;
int i = 0;

void Sphere::set(float radius, int sectors, int stacks) {
    this->radius = radius;
    this->sectorCount = sectors;
    if (sectors < MIN_SECTOR_COUNT)
        this->sectorCount = MIN_SECTOR_COUNT;
    this->stackCount = stacks;
    if (sectors < MIN_STACK_COUNT)
        this->sectorCount = MIN_STACK_COUNT;
    buildVertices();
}

 void Sphere::buildVertices()
 {
     const float PI = acos(-1);

     // clear memory of prev arrays
     clearArrays();

     float x, y, z, xy;                              // vertex position
     float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
     float s, t;                                     // texCoord

     float sectorStep = 2 * PI / sectorCount;
     float stackStep = PI / stackCount;
     float sectorAngle, stackAngle;

     for (int i = 0; i <= stackCount; ++i)
     {
         stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
         xy = radius * cosf(stackAngle);             // r * cos(u)
         z = radius * sinf(stackAngle);              // r * sin(u)

         // add (sectorCount+1) vertices per stack
         // the first and last vertices have same position and normal, but different tex coords
         for (int j = 0; j <= sectorCount; ++j)
         {
             sectorAngle = j * sectorStep;           // starting from 0 to 2pi

             // vertex position
             x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
             y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
             addVertex(x, y, z);

             // normalized vertex normal
             nx = x * lengthInv;
             ny = y * lengthInv;
             nz = z * lengthInv;
             addNormal(nx, ny, nz);

			 // vertex tex coord between [0, 1]
			 s = (float)j / sectorCount;
			 t = (float)i / stackCount;
			 addTexCoord(s, t);
         }
     }

     // indices
     //  k1--k1+1
     //  |  / |
     //  | /  |
     //  k2--k2+1
     unsigned int k1, k2;
     for (int i = 0; i < stackCount; ++i)
     {
         k1 = i * (sectorCount + 1);     // beginning of current stack
         k2 = k1 + sectorCount + 1;      // beginning of next stack

         for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
         {
             // 2 triangles per sector excluding 1st and last stacks
             if (i != 0)
             {
                 addIndices(k1, k2, k1 + 1);   // k1---k2---k1+1
             }

             if (i != (stackCount - 1))
             {
                 addIndices(k1 + 1, k2, k2 + 1); // k1+1---k2---k2+1
             }
         }
     }
     initializeColor();
 }

void Sphere::addVertex(float x, float y, float z) {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);    
}
void Sphere::addNormal(float x, float y, float z) {
    normals.push_back(x);
    normals.push_back(y);
    normals.push_back(z);
}
void Sphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3) {
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}


void Sphere::addTexCoord(float s, float t)
{
	texCoords.push_back(s);
	texCoords.push_back(t);
}

void Sphere::draw() {
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
            GL_STATIC_DRAW);
        break;
    case 1:
        glBufferData(GL_ARRAY_BUFFER,                   // target
            (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
            &vertices_green[0],   // ptr to vertex data
            GL_STATIC_DRAW);
        break;
    case 2:
        glBufferData(GL_ARRAY_BUFFER,                   // target
            (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
            &vertices_blue[0],   // ptr to vertex data
            GL_STATIC_DRAW);
        break;
    case 3:
        glBufferData(GL_ARRAY_BUFFER,                   // target
            (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
            &vertices_yellow[0],   // ptr to vertex data
            GL_STATIC_DRAW);
        break;
    case 5:
        glBufferData(GL_ARRAY_BUFFER,                   // target
            (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
            &vertices_gray[0],   // ptr to vertex data
            GL_STATIC_DRAW);
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

    //TexCoord VBO
    
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)texCoords.size() * sizeof(float), // data size, # of bytes
        &texCoords[0],   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage
    glEnableVertexAttribArray(aTexCoordLocation);
    glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    //EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        indices.size() * sizeof(unsigned int),             // data size, # of bytes
        &indices[0],               // ptr to index data
        GL_STATIC_DRAW);                   // usage
  
    // draw a sphere with VBO
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(mtxModel));
    glDrawElements(GL_TRIANGLES,                    // primitive type
        indices.size(),          // # of indices
        GL_UNSIGNED_INT,                 // data type
        0);                       // offset to indices
}

void Sphere::clearArrays() {
    std::vector<float>().swap(vertices);
    std::vector<unsigned int>().swap(indices);
}

void Sphere::initializeColor() {
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

std::vector<float> Sphere::computeFaceNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
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