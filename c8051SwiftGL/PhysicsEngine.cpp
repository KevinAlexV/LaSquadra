//
//  PhysicsEngine.cpp
//  c8051SwiftGL
//
//  Created by Thomas on 2022-04-05.
//

#include "PhysicsEngine.hpp"

#include <iostream>

PhysicsEngine* PhysicsEngine::instance = 0;

PhysicsEngine::PhysicsEngine():theWorld(0), isInit(false), bodies(new std::vector<b2Body*>()){}

PhysicsEngine::~PhysicsEngine()
{
    if(theWorld)
        delete theWorld;
    if(bodies)
        delete bodies;
}

void PhysicsEngine::init(b2Vec2 grav)
{
    gravity = grav;
    if(theWorld)
        delete theWorld;
    theWorld = new b2World(gravity);
    isInit = true;
}

void PhysicsEngine::update(float deltaTime)
{
    float timestep = 1000 / 60.0f;
    float cutoff = timestep / 2;
    if(isInit)
    {
        float remainingTime = deltaTime;
        do
        {
            //std::cerr << remainingTime << " " << timestep << std::endl;
            theWorld->Step(timestep, 6, 3);
            remainingTime -= timestep;
        }while(remainingTime > cutoff);
        //std::cerr << std::endl;
    }
}

b2BodyDef PhysicsEngine::CreatePhysicsBodyDef(b2BodyType type, float x, float y)
{
    b2BodyDef bodyDef;
    
    bodyDef.type = type;
    bodyDef.position.Set(x, y);
    
    return bodyDef;
}

b2Body* PhysicsEngine::CreatePhysicsBody(b2BodyDef def)
{
    b2Body* body = 0;
    
    if(isInit)
        body = theWorld->CreateBody(&def);
    
    return body;
}

b2PolygonShape PhysicsEngine::CreatePhysicsShape(float width, float height)
{
    b2PolygonShape shape;
    
    shape.SetAsBox(width / 2.0f, height / 2.0f);
    
    return shape;
}

b2Fixture* PhysicsEngine::CreatePhysicsFixture(b2Body* body, b2PolygonShape shape, float density)
{
    b2Fixture* fixture = 0;
    
    if(body)
        fixture = body->CreateFixture(&shape, density);
    
    return fixture;
}

bool PhysicsEngine::isInitialized() { return isInit; }

PhysicsEngine* PhysicsEngine::GetInstance()
{
    if(!instance)
        instance = new PhysicsEngine();
    
    return instance;
}
