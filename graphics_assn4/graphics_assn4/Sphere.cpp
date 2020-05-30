#include "Sphere.h"
#include "shaderinfo.h"
#include <math.h>
#include <iostream>

using namespace std;

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;
int i = 0;

Sphere::Sphere(float radius, int sectors, int stacks) {

}

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

     // tmp vertex definition (x,y,z,s,t)
     struct Vertex
     {
         float x, y, z, s, t;
     };
     std::vector<Vertex> tmpVertices;

     float sectorStep = 2 * PI / sectorCount;
     float stackStep = PI / stackCount;
     float sectorAngle, stackAngle;

     // compute all vertices first, each vertex contains (x,y,z,s,t) except normal
     for (int i = 0; i <= stackCount; ++i)
     {
         stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
         float xy = radius * cosf(stackAngle);       // r * cos(u)
         float z = radius * sinf(stackAngle);        // r * sin(u)

         // add (sectorCount+1) vertices per stack
         // the first and last vertices have same position and normal, but different tex coords
         for (int j = 0; j <= sectorCount; ++j)
         {
             sectorAngle = j * sectorStep;           // starting from 0 to 2pi

             Vertex vertex;
             vertex.x = xy * cosf(sectorAngle);      // x = r * cos(u) * cos(v)
             vertex.y = xy * sinf(sectorAngle);      // y = r * cos(u) * sin(v)
             vertex.z = z;                           // z = r * sin(u)
             vertex.s = (float)j / sectorCount;        // s
             vertex.t = (float)i / stackCount;         // t
             tmpVertices.push_back(vertex);
         }
     }

     // clear memory of prev arrays
     clearArrays();

     Vertex v1, v2, v3, v4;                          // 4 vertex positions and tex coords
     std::vector<float> n;                           // 1 face normal

     int i, j, k, vi1, vi2;
     int index = 0;                                  // index for vertex
     for (i = 0; i < stackCount; ++i)
     {
         vi1 = i * (sectorCount + 1);                // index of tmpVertices
         vi2 = (i + 1) * (sectorCount + 1);

         for (j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
         {
             // get 4 vertices per sector
             //  v1--v3
             //  |    |
             //  v2--v4
             v1 = tmpVertices[vi1];
             v2 = tmpVertices[vi2];
             v3 = tmpVertices[vi1 + 1];
             v4 = tmpVertices[vi2 + 1];

             // if 1st stack and last stack, store only 1 triangle per sector
             // otherwise, store 2 triangles (quad) per sector
             if (i == 0) // a triangle for first stack ==========================
             {
                 // put a triangle
                 addVertex(v1.x, v1.y, v1.z);
                 addVertex(v2.x, v2.y, v2.z);
                 addVertex(v4.x, v4.y, v4.z);

                 // put normal
                 n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);
                 for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                 {
                     addNormal(n[0], n[1], n[2]);
                 }

                 // put indices of 1 triangle
                 addIndices(index, index + 1, index + 2);

                 index += 3;     // for next
             }
             else if (i == (stackCount - 1)) // a triangle for last stack =========
             {
                 // put a triangle
                 addVertex(v1.x, v1.y, v1.z);
                 addVertex(v2.x, v2.y, v2.z);
                 addVertex(v3.x, v3.y, v3.z);

                 // put normal
                 n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                 for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                 {
                     addNormal(n[0], n[1], n[2]);
                 }

                 // put indices of 1 triangle
                 addIndices(index, index + 1, index + 2);

                 index += 3;     // for next
             }
             else // 2 triangles for others ====================================
             {
                 // put quad vertices: v1-v2-v3-v4
                 addVertex(v1.x, v1.y, v1.z);
                 addVertex(v2.x, v2.y, v2.z);
                 addVertex(v3.x, v3.y, v3.z);
                 addVertex(v4.x, v4.y, v4.z);

                 // put normal
                 n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                 for (k = 0; k < 4; ++k)  // same normals for 4 vertices
                 {
                     addNormal(n[0], n[1], n[2]);
                 }

                 // put indices of quad (2 triangles)
                 addIndices(index, index + 1, index + 2);
                 addIndices(index + 2, index + 1, index + 3);

                 index += 4;     // for next
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

    //EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        indices.size() * sizeof(unsigned int),             // data size, # of bytes
        &indices[0],               // ptr to index data
        GL_STATIC_DRAW);                   // usage
  
    // draw a sphere with VBO
    glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, glm::value_ptr(mtxView));
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
    cout << "Vertices : " << vertices.size() << endl;
    cout << "Color : " << vertices_gray.size() << endl;
    cout << "Normal : " << normals.size() << endl << endl;
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