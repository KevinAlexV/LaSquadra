//
//  SceneManager.hpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-17.
//

#ifndef SceneManager_hpp
#define SceneManager_hpp

#include <stdio.h>
#include "Scene.hpp"

class SceneManager{
    
    Scene* scene;
    vector<GLuint> textureIds;
    
public:
    enum SceneName {
        MENU,
        MAZE,
        CHEMISTRY_LAB,
        OBSTACLE_COURSE
    };
    
    void reset();
    void assignScene(SceneName);
    void pan(float, float);
    void movePlayer(int);
    void update();
    void draw(float, GLint, GLint);
    void pushTexture(GLuint);
    bool checkGoalCondition();
    void setSceneFont(char *);
    
    void handleDoubleTap(float inputX, float inputY, float vpWidth, float vpHeight, float sWidth, float sHeight);
    
    float getGameTime();
    string getWinMsg();
    
private:
    SceneName curSceneName;
};

#endif /* SceneManager_hpp */
