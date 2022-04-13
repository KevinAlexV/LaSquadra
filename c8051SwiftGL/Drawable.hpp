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
#include "PhysicsEngine.hpp"

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
    void assignPhysicsBody(b2Body* body);
    void updateTransform();
    glm::mat4 draw(glm::mat4 mvp);
    glm::mat4 debug_draw(glm::mat4 mvp);
    
    int getDebugNumIndices();
    int* getDebugIndices();
    
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
    b2Body* getPhysicsBody();
    
    bool isUI = false;
protected:
    
    GLuint textureListIndex;
    int debug_numIndices, numVertices, numNormals, numTexCoords, numIndices;
    vector<int> indices;
    vector<float> vertices;
    vector<float> normals;
    vector<float> texCoords;
    vector<int> debug_indices;
    vector<float> debug_vertices;
    vector<float> debug_normals;
    vector<float> debug_texCoords;
    glm::mat4 transformMatrix;
    
    b2Body* physicsBody;
};

#endif
