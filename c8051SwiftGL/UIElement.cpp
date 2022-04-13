//
//  UITimer.cpp
//  c8051SwiftGL
//
//  Created by Kevin Vilanova on 2022-03-28.
//

#include "UIElement.hpp"

//Constructor for UITimer, which is a child of drawable.
//When the constructor is called, ensure you specify the textureID that the texture is stored within (based on the texture list in glesrenderer or renderer.mm),
//and the verts, normals, texture coords, and indices array size.
UIElement::UIElement(float scaleX, float scaleY, int textureListIndex):Drawable(textureListIndex, 4, 4, 4, 6)
{
    
    isUI = true;
    //cout << "\nUI X Scale: " << scaleX;
    //cout << "\nUI Y Scale: " << scaleY << endl;
    xScale = scaleX;
    yScale = scaleY;
    
    loadVertices(0);
    loadNormals(0);
    loadTextureCoords(0);
    loadIndices(0);
}

bool UIElement::loadVertices(void* info)
{
    
    //X, Y, Z
    float cubeVerts[] =
    {
        -1.0f * xScale, -1.0f * yScale, 0.0f, // Lower left corner
        1.0f * xScale, 1.0f * yScale, 0.0f, //Upper right corner
        -1.0f * xScale, 1.0f * yScale, 0.0f, // Upper left corner
        1.0f * xScale, -1.0f * yScale, 0.0f, //lower right corner
    };
    
    for(int i = 0; i < numVertices * 3; i++)
    {
        //should be 4
        //cout <<"\nverts: "<< numVertices;
        //vertices[i] = cubeVerts[i];
        vertices.push_back(cubeVerts[i]);
    }
    
    return true;
}

bool UIElement::loadNormals(void* info)
{
    float cubeNormals[] =
    {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };
    
    
    
    for(int i = 0; i < numNormals * 3; i++)
    {
        //cout << "\nnormals: " << numNormals;
        //normals[i] = cubeNormals[i];
        normals.push_back(cubeNormals[i]);
    }
    
    return true;
}

bool UIElement::loadTextureCoords(void* info)
{
    //Texture mappings
    float cubeTex[] =
    {
        0.0f, 0.0f, // Lower left corner
        1.0f, 1.0f,//Upper right corner
        0.0f, 1.0f,//Upper left corner
        1.0f, 0.0f,// Lower right corner
    };
    
    for(int i = 0; i < numTexCoords * 2; i++)
    {
        
        //cout << "text coords: " << numTexCoords;
        //texCoords[i] = cubeTex[i];
        texCoords.push_back(cubeTex[i]);
    }
    
    return true;
}

bool UIElement::loadIndices(void* info)
{
    //should be 6
    int cubeIndices[] =
    {
        0, 1, 2,
        0, 3, 1,
    };
    
    for(int i = 0; i < numIndices; i++)
    {
        
        //cout << "\nIndices: " << numIndices;
        //indices[i] = cubeIndices[i];
        indices.push_back(cubeIndices[i]);
    }
    
    return true;
}

float UIElement::getXScale()
{
    return xScale;
}
float UIElement::getYScale()
{
    return yScale;
}
/*
float UIElement::getWidth()
{
    return (vertices[2] - vertices[1]);
    
}

float UIElement::getHeight()
{
    return (vertices[2] - vertices[0]);
    
}
*/
