//
//  Scene.hpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-17.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <OpenGLES/ES3/gl.h>
#include "glm-master/glm/ext.hpp"
#include <stdio.h>
#include <chrono>
#include "Cube.hpp"
#include "Sphere.hpp"
#include "Cylinder.hpp"
#include "UIElement.hpp"
#include "Camera.hpp"
#include "Maze.hpp"
#include "Potion.hpp"

using namespace glm;

class Scene {
    
protected:
    Drawable* playerDrawable;
    Drawable* goal;
    int playerDir = -1;
    vector<Drawable*> drawables;
    void addDrawable(Drawable* d);
    Camera* camera;
    mat4 mvp;
    mat4 mvpUI;
    mat3 normalMatrix;
    const float PLAYER_SPEED = 0.01f, PLAYER_SLIP = 0.001f;
    void updateTransform();
    std::chrono::time_point<std::chrono::steady_clock> now;
    std::chrono::time_point<std::chrono::steady_clock> lastFrame;
    
public:
    Scene();
    ~Scene();
    virtual void reset();
    static float normalize(float, float, float);
    int sceneGoalCondition;
    
    float timeLeft;
    
    bool gameStarted = false;
    bool sceneWon = false;
    virtual void pan(float, float);
    virtual void handleDoubleTap(float, float, float, float, float, float);
    virtual void movePlayer(int);
    virtual void update();
    virtual bool achievedGoal() = 0;
    virtual void draw(vector<GLuint>, float, GLint, GLint);
    virtual void loadModels();
    float getTimeLeft();
    
};

//----------------------- Maze minigame -----------------------

class MazeScene : public Scene {
    const int WALL_NUM = 8;
    
public:
    void reset() override;
    void loadModels() override;
    void update() override;
    void pan(float, float) override;
    void movePlayer(int) override;
    bool isAllCoinsCollected();
    bool achievedGoal() override;
    void addWall(bool, float, float, float, int = 1);
    void addTimer(float, float, int = 1);
    void addCoin(float, float, float, float, int, int = 30);
    void addGoal(float, float, float, float, int, int = 30);
    int collisionCheck(float posX, float posY);
    //Given position and cell is there a collision?
    int wallCheck(int row, int column, float posX, float posY);
    
    
protected:
    vector<Drawable*> coinDrawables;
    Maze* maze;
};

//----------------------- Potion minigame -----------------------

class PotionScene : public Scene {
public:
    void reset() override;
    void loadModels() override;
    void update() override;
    void movePlayer(int) override;
    void handleDoubleTap(float, float, float, float, float, float) override;
    //void movePlayer(int) override;
    bool achievedGoal() override;
private://for add potion, change int to proper index of texture.
    void addPotion(float, float, int=1);
    vector<Potion*> potions;
    
    int potionsPerRow;
    int potionsPerColumn;
    int prevSelection = -1;
    vec2 selected = vec2(0.f,0.f);

protected:
    //vector<Drawable*> potionHighlights;
};

#endif /* Scene_hpp */
