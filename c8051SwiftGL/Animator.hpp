//
//  Animator.hpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-16.
//

#ifndef Animator_hpp
#define Animator_hpp

#include <stdlib.h>
#include <chrono>
#include "Transform.hpp"
#include "Maze.hpp"

using namespace std;

class Animator{
    std::chrono::time_point<std::chrono::steady_clock> lastFrame;
    bool enabled = false;
    float slip = 0, buildupSpeed = 0;
    
protected:
    Transform* contextTransform = nullptr;
    Transform* transformSpeed = nullptr;
    
public:
    Animator();
    Animator(Transform*);
    void assignTransform(Transform*);
    void assignTransformSpeed(Transform*);
    virtual void update();
    void setEnabled(bool);
    void setBuildupSpeed(float);
    Transform* getTransformSpeed();
    bool isMoving();
};

class MazeEnemy : public Animator {
    int dir;
    int posX = 0, posY = 0;
    Maze* maze = nullptr;
    
public:
    MazeEnemy();
    MazeEnemy(Transform*);
    void referenceMaze(Maze*);
    void update() override;
    void setDirection(int);
};

#endif /* Animator_hpp */
