//
//  Camera.cpp
//  c8051SwiftGL
//
//  Created by Thomas Roughley on 2022-02-22.
//

#include "Camera.hpp"
#include "glm-master/glm/ext.hpp"

Camera* Camera::INSTANCE = new Camera();

glm::vec4 Camera::UP = glm::vec4(0,1,0,0);
glm::vec4 Camera::RIGHT = glm::vec4(1,0,0,0);
glm::vec4 Camera::FACING = glm::vec4(0,0,1,0);

Camera* Camera::GetInstance()
{
    if(INSTANCE == 0)
        INSTANCE = new Camera();
    
    return INSTANCE;
}

Camera::Camera()
:transform(new Transform()), facing(FACING), right(RIGHT), up(UP)
{
    
}

Camera::~Camera()
{
    delete transform;
}

void Camera::lookAt(glm::vec4 point)
{
}

void Camera::rotate(float angle, glm::vec3 axis)
{
    glm::quat rot(1,0,0,0);
    rot = glm::rotate(rot, angle, axis);
    transform->rotate(glm::eulerAngles(rot));
    
    glm::quat rotConj = glm::conjugate(rot);
    facing = rotConj * facing;
    right = rotConj * right;
    up = rotConj * up;
    
    /*glm::mat4x4 rot = glm::mat4_cast(glm::conjugate(rotation));
    
    glm::vec4 newFacing = rot * facing;
    glm::vec4 newRight = rot * right;
    glm::vec4 newUp = rot * up;
    
    facing = newFacing;
    right = newRight;
    up = newUp;*/
}

void Camera::setAspectRatio(float ratio)
{
    aspectRatio = ratio;
}

void Camera::setFieldOfView(float fov)
{
    fieldOfView = fov;
}

void Camera::setNearZ(float newZ)
{
    nearZ = newZ;
}

void Camera::setFarZ(float newZ)
{
    farZ = newZ;
}

void Camera::reset()
{
}

glm::mat4x4 Camera::getPerspective() {return glm::perspective(fieldOfView, aspectRatio, nearZ, farZ); }
Transform* Camera::getTransform() { return transform; }
glm::vec4 Camera::getUp() { return up; }
glm::vec4 Camera::getRight() { return right; }
glm::vec4 Camera::getLookAt() { return facing; }
float Camera::getAspectRatio() { return aspectRatio; }
float Camera::getFieldOfView() { return fieldOfView; }
float Camera::getNearZ() { return nearZ; }
float Camera::getFarZ() { return farZ; }
