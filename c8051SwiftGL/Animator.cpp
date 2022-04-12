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

void MazeEnemy::referenceMaze(Maze* maze){
    this->maze = maze;
}

void MazeEnemy::update(){
    Animator::update();
    
}
