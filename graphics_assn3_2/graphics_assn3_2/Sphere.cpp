#include "Sphere.h"
#include "main.h"


const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;
int i = 0;

Sphere::Sphere(float radius, int sectors, int stacks) : interleavedStride(32)
{
    set(radius, sectors, stacks);
}

///////////////////////////////////////////////////////////////////////////////
// setters
///////////////////////////////////////////////////////////////////////////////
void Sphere::set(float radius, int sectors, int stacks)
{
    this->radius = radius;
    this->sectorCount = sectors;
    if (sectors < MIN_SECTOR_COUNT)
        this->sectorCount = MIN_SECTOR_COUNT;
    this->stackCount = stacks;
    if (sectors < MIN_STACK_COUNT)
        this->sectorCount = MIN_STACK_COUNT;
    buildVertices();
}

void Sphere::setRadius(float radius)
{
    if (radius != this->radius)
        set(radius, sectorCount, stackCount);
}

void Sphere::setSectorCount(int sectors)
{
    if (sectors != this->sectorCount)
        set(radius, sectors, stackCount);
}

void Sphere::setStackCount(int stacks)
{
    if (stacks != this->stackCount)
        set(radius, sectorCount, stacks);
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

                // put tex coords of triangle
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v4.s, v4.t);

                /*
                // put normal
                n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);
                for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }
                */

                // put indices of 1 triangle
                addIndices(index, index + 1, index + 2);

                // indices for line (first stack requires only vertical line)
                lineIndices.push_back(index);
                lineIndices.push_back(index + 1);

                index += 3;     // for next
            }
            else if (i == (stackCount - 1)) // a triangle for last stack =========
            {
                // put a triangle
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);

                // put tex coords of triangle
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v3.s, v3.t);

                // put normal
                /*
                n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }
                */

                // put indices of 1 triangle
                addIndices(index, index + 1, index + 2);

                // indices for lines (last stack requires both vert/hori lines)
                lineIndices.push_back(index);
                lineIndices.push_back(index + 1);
                lineIndices.push_back(index);
                lineIndices.push_back(index + 2);

                index += 3;     // for next
            }
            else // 2 triangles for others ====================================
            {
                // put quad vertices: v1-v2-v3-v4
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);
                addVertex(v4.x, v4.y, v4.z);

                // put tex coords of quad
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v3.s, v3.t);
                addTexCoord(v4.s, v4.t);

                /*
                // put normal
                n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                for (k = 0; k < 4; ++k)  // same normals for 4 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }
                */

                // put indices of quad (2 triangles)
                addIndices(index, index + 1, index + 2);
                addIndices(index + 2, index + 1, index + 3);

                // indices for lines
                lineIndices.push_back(index);
                lineIndices.push_back(index + 1);
                lineIndices.push_back(index);
                lineIndices.push_back(index + 2);

                index += 4;     // for next
            }
        }
    }

    // generate interleaved vertex array as well
    //buildInterleavedVertices();
    //cout << vertices.size() << endl;
    //cout << indices.size() << endl;
}

/*
void Sphere::buildInterleavedVertices()
{
    std::vector<float>().swap(interleavedVertices);

    std::size_t i, j;
    std::size_t count = vertices.size();
    for (i = 0, j = 0; i < count; i += 3, j += 2)
    {
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i + 1]);
        interleavedVertices.push_back(vertices[i + 2]);

        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i + 1]);
        interleavedVertices.push_back(normals[i + 2]);

        interleavedVertices.push_back(texCoords[j]);
        interleavedVertices.push_back(texCoords[j + 1]);
    }
}
*/

void Sphere::addVertex(float x, float y, float z)
{
    //cout << x <<" "<< y << " " << z;
    
    //vertices.push_back(glm::vec3(x, y, z));
    
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
    
}

void Sphere::addNormal(float nx, float ny, float nz)
{
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}

void Sphere::addTexCoord(float s, float t)
{
    texCoords.push_back(s);
    texCoords.push_back(t);
}

void Sphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    
    //cout << i1 << " " << i2 << " " << i3 << endl;
    //indices.push_back(glm::uvec3(i1,i2,i3));
    
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
    
}

void Sphere::draw()
{
    
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        &vertices[0],   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        &vertices[0],   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        indices.size() * sizeof(unsigned int),             // data size, # of bytes
        &indices[0],               // ptr to index data
        GL_STATIC_DRAW);                   // usage
  
    // draw a sphere with VBO
    glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(mtxView));
    glDrawElements(GL_TRIANGLES,                    // primitive type
        indices.size(),          // # of indices
        GL_UNSIGNED_INT,                 // data type
        0);                       // offset to indices

}

void Sphere::clearArrays()
{
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
}