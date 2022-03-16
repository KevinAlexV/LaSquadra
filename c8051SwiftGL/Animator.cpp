//
//  Animator.cpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-16.
//

#include "Animator.hpp"

Animator::Animator(vec3 rotateSpeed){
    this->rotateSpeed = rotateSpeed;
}

void Animator::assignTransform(Transform* contextTransform){
    this->contextTransform = contextTransform;
}

void Animator::update(){
    if(enabled){
        auto elapsedTime = chrono::duration_cast<std::chrono::milliseconds>(chrono::steady_clock::now() - lastFrame).count();
        contextTransform->rotate(vec3(radians(rotateSpeed.x * elapsedTime), radians(rotateSpeed.y * elapsedTime), radians(rotateSpeed.z * elapsedTime)));
    }
}

void Animator::setEnabled(bool enabled){
    this->enabled = enabled;
}