//
//  Camera.cpp
//  c8051SwiftGL
//
//  Created by Thomas Roughley on 2022-02-22.
//

#include "Camera.hpp"

glm::vec4 Camera::UP = glm::vec4(0,1,0,0);
glm::vec4 Camera::RIGHT = glm::vec4(1,0,0,0);
glm::vec4 Camera::FACING = glm::vec4(0,0,1,0);

Camera::Camera()
:transform(new Transform()), facing(glm::vec4(0,0,1,0)), right(glm::vec4(1,0,0,0)), up(glm::vec4(0,1,0,0))
{
    
}

Camera::~Camera()
{
    delete transform;
}

void Camera::lookAt(glm::vec4 point)
{
    glm::vec4 lookAt = point - transform->getPosition();
    lookAt -= facing;
    
    
}

void Camera::rotate(float angle, glm::vec3 axis)
{
    glm::quat rotation = transform->rotate(angle, axis);
    glm::quat rotConj = glm::conjugate(rotation);
    glm::vec4 newFacing = rotation * facing * rotConj;
    glm::vec4 newRight = rotation * right * rotConj;
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
    if(newZ > farZ)
    {
        nearZ = farZ;
        farZ = newZ;
    }
    else
    {
        nearZ = newZ;
    }
}

void Camera::setFarZ(float newZ)
{
    if(newZ < nearZ)
    {
        farZ = nearZ;
        nearZ = newZ;
    }
    else
    {
        farZ = newZ;
    }
}

glm::vec4 Camera::getUp() { return up; }
glm::vec4 Camera::getRight() { return right; }
glm::vec4 Camera::getLookAt() { return facing; }
float Camera::getAspectRatio() { return aspectRatio; }
float Camera::getFieldOfView() { return fieldOfView; }
float Camera::getNearZ() { return nearZ; }
float Camera::getFarZ() { return farZ; }