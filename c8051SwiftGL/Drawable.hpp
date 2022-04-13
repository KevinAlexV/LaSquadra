#ifndef Renderable_hpp
#define Renderable_hpp

#include <stdio.h>
#include <vector>
#include "GameObject.hpp"
#include <OpenGLES/ES3/gl.h>
#include "glm-master/glm/ext.hpp"
#include "glm-master/glm/gtc/quaternion.hpp"
#include "glm-master/glm/gtx/quaternion.hpp"
#include "Animator.hpp"

using namespace std;

class Drawable : public GameObject
{
public:
    Drawable(int textureListIndex, int numVertices, int numNormals, int numTexCoords, int numIndices);
    Drawable(const Drawable& obj);
    virtual ~Drawable();
    
    // Load data for vertices, normals, and texture coordinates
    // void-pointer allows for type-casting for flexibilty in dealing with info
    virtual bool loadVertices(void* info) = 0;
    virtual bool loadNormals(void* info) = 0;
    virtual bool loadTextureCoords(void* info) = 0;
    virtual bool loadIndices(void* info) = 0;
    
    void assignAnimator(Animator*);
    void updateTransform();
    void changeTexture(int);
    glm::mat4 draw(glm::mat4 mvp);
    
    int getNumVertices();
    int getNumNormals();
    int getNumTexCoords();
    int getNumIndices();
    int* getIndices();
    float* getVertices();
    float* getNormals();
    float* getTextureCoords();
    int getTextureListIndex();
    Animator* anim = NULL;
    
    bool isUI = false;
protected:
    
    GLuint textureListIndex;
    int numVertices, numNormals, numTexCoords, numIndices;
    vector<int> indices;
    vector<float> vertices;
    vector<float> normals;
    vector<float> texCoords;
    glm::mat4 transformMatrix;
};

#endif
