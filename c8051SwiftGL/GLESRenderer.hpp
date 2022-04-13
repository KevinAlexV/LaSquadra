//
//  Copyright © Borna Noureddin. All rights reserved.
//

#ifndef GLESRenderer_hpp
#define GLESRenderer_hpp

#include <stdlib.h>

#include <OpenGLES/ES3/gl.h>

#include "glm-master/glm/ext.hpp"
#include "glm-master/glm/gtc/quaternion.hpp"
#include "glm-master/glm/gtx/quaternion.hpp"
#include "Drawable.hpp"
#include "Maze.hpp"
#include "Cube.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"
#include "SceneManager.hpp"

using namespace std;
using namespace glm;

// Uniform variables
enum
{
    UNIFORM_MODELVIEWPROJECTION_MATRIX,
    UNIFORM_NORMAL_MATRIX,
    UNIFORM_PASSTHROUGH,
    UNIFORM_SHADEINFRAG,
    UNIFORM_TEXTURE,
    NUM_UNIFORMS
};

// Attributes
enum
{
    ATTRIB_VERTEX,
    ATTRIB_NORMAL,
    NUM_ATTRIBUTES
};

class GLESRenderer
{
public:
    bool isRotating;// publicly available member variable (in this case whether the cube is auto-rotating
    float panX = 0, panY = 0;

    // Constructor and destructor
    GLESRenderer(const char *vertexShaderFile = NULL, const char *fragmentShaderFile = NULL,
                 /*GLubyte *spriteData = 0, size_t width = -1, size_t height = -1,*/ GLubyte **spriteData = 0, size_t *width = 0, size_t *height = 0);
    ~GLESRenderer();
    
    void SetViewport(int width, int height) { vpWidth = width; vpHeight = height; }
    void Update();
    void Draw();
    void reset();
    bool achievedGoal();
    float getGameTime();
    string getWinMsg();
    void setPlayerDir(int);
    void handleDoubleTap(float inputX, float inputY, float sWidth, float sHeight);
    int getSceneGoal();

private:
    int vpWidth, vpHeight;  // viewport width/height
    bool updatedOnce = false;
    SceneManager sceneManager;

    // GL variables
    GLint uniforms[NUM_UNIFORMS];
    GLuint programObject;
    GLuint groundTexture;
    glm::mat4 mvp;
    glm::mat3 normalMatrix;
    Camera* camera;

    // angle of rotation of cube
    float rotAngle;

    void LoadModels(int sceneNum = 1);

    bool SetupShaders(const char *vertexShaderFile, const char *fragmentShaderFile);
    GLuint SetupTexture(GLubyte *spriteData, size_t width, size_t height);

    char *LoadShaderFile(const char *shaderFileName);
    GLuint LoadShader(GLenum type, const char *shaderSrc);
    GLuint LoadProgram(const char *vertShaderSrc, const char *fragShaderSrc);
};

#endif /* GLESRenderer_hpp */
