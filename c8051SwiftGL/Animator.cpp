//
//  Animator.cpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-16.
//

#include "Animator.hpp"

Animator::Animator(){
    transformSpeed = new Transform();
    transformSpeed->setScale(vec3(0.f, 0.f, 0.f));
}

Animator::Animator(Transform* transformSpeed){
    this->transformSpeed = transformSpeed;
}

void Animator::assignTransform(Transform* contextTransform){
    this->contextTransform = contextTransform;
}

void Animator::assignTransformSpeed(Transform* transformSpeed){
    this->transformSpeed = transformSpeed;
}

void Animator::update(){
    auto elapsedTime = chrono::duration_cast<std::chrono::milliseconds>(chrono::steady_clock::now() - lastFrame).count() * 0.01f;
    if(enabled && slip < buildupSpeed){
        slip += elapsedTime * 10.f;
        if(slip > buildupSpeed)
            slip = buildupSpeed;
    }
    else if(!enabled && slip > 0){
        slip -= elapsedTime * 10.f;
        if(slip < 0)
            slip = 0;
    }
    
    if(enabled || (!enabled && slip > 0)){
        float bRatio = ((buildupSpeed > 0) ? slip / buildupSpeed : 1);
        vec3 positionSpeed  = transformSpeed->getPosition(), rotationSpeed = transformSpeed->getAngles();
        contextTransform->translate(vec3(bRatio * positionSpeed.x * elapsedTime, 0.f, bRatio * positionSpeed.z * elapsedTime));
        contextTransform->rotate(vec3(rotationSpeed.x * elapsedTime, rotationSpeed.y * elapsedTime, rotationSpeed.z * elapsedTime));
    }
    
    lastFrame = chrono::steady_clock::now();
}

void Animator::setEnabled(bool enabled){
    this->enabled = enabled;
}

void Animator::setBuildupSpeed(float ms){
    this->buildupSpeed = ms;
}

Transform* Animator::getTransformSpeed(){
    return transformSpeed;
}

bool Animator::isMoving(){
    return slip > 0;
}

MazeEnemy::MazeEnemy() : Animator() {
    srand(NULL);
}

MazeEnemy::MazeEnemy(Transform* transform) : Animator(transform) {
    srand(NULL);
}

void MazeEnemy::referenceMaze(Maze* maze){
    this->maze = maze;
    /*bool wallsVisible[2] = {
        !maze->maze[0].getWallVisible(2),
        !maze->maze[0].getWallVisible(1)
    };
    if(wallsVisible[0] && wallsVisible[1])
        setDirection(rand() % 2 + 1);
    else if(wallsVisible[1])
        setDirection(1);
    else if(wallsVisible[0])
        setDirection(2);*/
}

void MazeEnemy::update(){
    Animator::update();
    if(maze != nullptr){
        
    }
}

void MazeEnemy::setDirection(int direction){
    this->dir = direction;
    transformSpeed = new Transform();
    vec3 posSpeed = vec3(0.f, 0.f, 0.f);
    switch(dir){
        case 0:
            posSpeed.z = -0.05f;
            break;
        case 1:
            posSpeed.x = 0.05f;
            break;
        case 2:
            posSpeed.z = 0.05f;
            break;
        case 3:
            posSpeed.x = -0.05f;
            break;
    }
    transformSpeed->setPosition(posSpeed);
}
