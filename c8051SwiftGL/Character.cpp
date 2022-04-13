//
//  Character.cpp
//  c8051SwiftGL
//

#include "Character.hpp"
#include "ObjModelReader.h"

Character::Character(int textureListIndex, float radius)
:Drawable(textureListIndex, 24, 24, 24, 36)
{
    this->radius = radius;
    loadAll();
    
    //loadVertices(0);
    //loadNormals(0);
    //loadTextureCoords(0);
    //loadIndices(0);
}

void Character::loadAll() {
    ObjModelReader::readObjFile("character", indices, vertices, normals, texCoords);
    
    if (vertices.size() != 0)
    {
        float maxX = 0;
        float minY = MAXFLOAT;
        for (int i = 0; i < vertices.size() / 3; i++) {
            float x = vertices[i * 3];
            float y = vertices[i * 3 + 1];
            if (x > maxX) {
                maxX = x;
            }
            if (y < minY) {
                minY = y;
            }
        }
        float scale = radius /  maxX;
        float moveY = minY - radius;
        
        for (int i = 0; i < vertices.size(); i++) {
            vertices[i] = vertices[i] * scale;
            if (i % 3 == 1) { // y
                vertices[i] += moveY;
            }
        }
    }
}

bool Character::loadVertices(void* info)
{
    /*for(int i = 0; i < numVertices * 3; i++)
    {
        vertices[i] = cubeVerts[i];
    }*/
    
    return true;
}

bool Character::loadNormals(void* info)
{
    /*for(int i = 0; i < numNormals * 3; i++)
    {
        normals[i] = cubeNormals[i];
    }*/
    
    return true;
}

bool Character::loadTextureCoords(void* info)
{
    /*for(int i = 0; i < numTexCoords * 2; i++)
    {
        texCoords[i] = cubeTex[i];
    }*/
    
    return true;
}

bool Character::loadIndices(void* info)
{
    /*for(int i = 0; i < numIndices; i++)
    {
        indices[i] = cubeIndices[i];
    }*/
    
    return true;
}
