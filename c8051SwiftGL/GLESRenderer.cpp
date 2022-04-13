//
//  Copyright © Borna Noureddin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <iostream>
#include "glm-master/glm/gtx/string_cast.hpp"
#include "GLESRenderer.hpp"


// ----------------------------------------------------------------
// Constructor and destructor
// ----------------------------------------------------------------
GLESRenderer::GLESRenderer(const char *vertexShaderFile, const char *fragmentShaderFile,
                           /*GLubyte *spriteData, size_t width, size_t height*/ GLubyte **spriteData, size_t *width, size_t *height)
{
    int randomSceneNum = (rand()%2) + 1;
    randomSceneNum = 1;                   // commented line for hard-coded scene value for LOADING THE FIRST SCENE
    LoadModels(randomSceneNum);

    if (vertexShaderFile && fragmentShaderFile)
    {
        if (!SetupShaders(vertexShaderFile, fragmentShaderFile))
            return;
    }
    

    //If adding more textures, change the 'i' max value to register each one.
    for(int i = 0; i < 15; i++){
        GLuint textureId = SetupTexture(spriteData[i], width[i], height[i]);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i(uniforms[UNIFORM_TEXTURE], 0);
        sceneManager.pushTexture(textureId);
        //cout << "Texture id: " << textureId << endl;
    }

    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
    glEnable(GL_DEPTH_TEST);

    rotAngle = 0.0f;
}

GLESRenderer::~GLESRenderer()
{
    glDeleteProgram(programObject);
}

// ----------------------------------------------------------------
// Update and Draw public methods
// ----------------------------------------------------------------
void GLESRenderer::Update()
{
    if (panX != 0 || panY != 0)
        sceneManager.pan(panX * 5, panY * 5);
    
    sceneManager.update();
}

void GLESRenderer::Draw()
{
    glUniform1i(uniforms[UNIFORM_PASSTHROUGH], false);
    glUniform1i(uniforms[UNIFORM_SHADEINFRAG], true);

    glViewport(0, 0, vpWidth, vpHeight);
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUseProgram ( programObject );

    float aspectRatio = ((vpHeight != 0 && vpWidth != 0) ? (float)vpWidth / (float)vpHeight : 3 / 4);
    sceneManager.draw((float)vpWidth / (float)vpHeight, uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX], uniforms[UNIFORM_NORMAL_MATRIX]);
}

void GLESRenderer::reset(){
    sceneManager.reset();
}

void GLESRenderer::setPlayerDir(int playerDir){
    sceneManager.movePlayer(playerDir);
}

// ========================================================================================


// ----------------------------------------------------------------
// Model loading - ADD RENDERABLES HERE
// ----------------------------------------------------------------
void GLESRenderer::LoadModels(int sceneNum)
{
    switch(sceneNum) {
        case 1:
            sceneManager.assignScene(sceneManager.MAZE);
            break;
        case 2:
            sceneManager.assignScene(sceneManager.CHEMISTRY_LAB);
            break;
    }
}

// ========================================================================================


// ----------------------------------------------------------------
// Shader loading
// ----------------------------------------------------------------
bool GLESRenderer::SetupShaders(const char *vertexShaderFile, const char *fragmentShaderFile)
{
    // Load shaders
    char *vShaderStr = LoadShaderFile(vertexShaderFile);
    char *fShaderStr = LoadShaderFile(fragmentShaderFile);
    programObject = LoadProgram(vShaderStr, fShaderStr);
    if (programObject == 0)
        return false;
    
    // Set up uniform variables
    uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX] = glGetUniformLocation(programObject, "modelViewProjectionMatrix");
    uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(programObject, "normalMatrix");
    uniforms[UNIFORM_PASSTHROUGH] = glGetUniformLocation(programObject, "passThrough");
    uniforms[UNIFORM_SHADEINFRAG] = glGetUniformLocation(programObject, "shadeInFrag");
    uniforms[UNIFORM_TEXTURE] = glGetUniformLocation(programObject, "texSampler");

    return true;
}

GLuint GLESRenderer::SetupTexture(GLubyte *spriteData, size_t width, size_t height)
{
    GLuint texName;
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
    
    return texName;
}

char *GLESRenderer::LoadShaderFile(const char *shaderFileName)
{
    FILE *fp = fopen(shaderFileName, "rb");
    if (fp == NULL)
        return NULL;

    fseek(fp , 0 , SEEK_END);
    long totalBytes = ftell(fp);
    fclose(fp);

    char *buf = (char *)malloc(totalBytes+1);
    memset(buf, 0, totalBytes+1);

    fp = fopen(shaderFileName, "rb");
    if (fp == NULL)
        return NULL;

    size_t bytesRead = fread(buf, totalBytes, 1, fp);
    fclose(fp);
    if (bytesRead < 1)
        return NULL;

    return buf;
}

GLuint GLESRenderer::LoadShader(GLenum type, const char *shaderSrc)
{
    GLuint shader = glCreateShader(type);
    if (shader == 0)
        return 0;
    
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char *infoLog = (char *)malloc(sizeof ( char ) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            std::cerr << "*** SHADER COMPILE ERROR:" << std::endl;
            std::cerr << infoLog << std::endl;
            free(infoLog);
        }
        glDeleteShader ( shader );
        return 0;
    }
    
    return shader;
}

GLuint GLESRenderer::LoadProgram(const char *vertShaderSrc, const char *fragShaderSrc)
{
    GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vertShaderSrc);
    if (vertexShader == 0)
        return 0;
    
    GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragShaderSrc);
    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return 0;
    }
    
    GLuint programObject = glCreateProgram();
    if (programObject == 0)
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }
    
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);
    
    GLint linked;
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            std::cerr << "*** SHADER LINK ERROR:" << std::endl;
            std::cerr << infoLog << std::endl;
            free(infoLog);
        }
        glDeleteProgram(programObject);
        return 0;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return programObject;
}

bool GLESRenderer::achievedGoal(){
    return sceneManager.checkGoalCondition();
}

float GLESRenderer::getGameTime()
{
    return sceneManager.getGameTime();
}

string GLESRenderer::getWinMsg(){
    return sceneManager.getWinMsg();
}

void GLESRenderer::handleDoubleTap(float inputX, float inputY, float sWidth, float sHeight)
{
    sceneManager.handleDoubleTap(inputX, inputY, vpWidth, vpHeight, sWidth, sHeight);
}

int GLESRenderer::getSceneGoal(){
    return sceneManager.getSceneGoal();
}
