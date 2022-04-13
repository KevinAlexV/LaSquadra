//
//  PhysicsEngine.hpp
//  c8051SwiftGL
//
//  Created by Thomas on 2022-04-05.
//

#ifndef PhysicsEngine_hpp
#define PhysicsEngine_hpp

#include <stdio.h>
#include <vector>
#include "include/box2d/box2d.h"

class PhysicsObject
{
public:
    PhysicsObject(int id);
    ~PhysicsObject();
    
    int getType();
protected:
private:
    int objectType;
};

class ContactHandler : public b2ContactListener
{
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
public:
};

class PhysicsEngine
{
public:
    ~PhysicsEngine();
    
    void init(b2Vec2 gravity);
    void update(float deltaTime);
    
    b2BodyDef CreatePhysicsBodyDef(b2BodyType type, float x, float y);
    b2Body* CreatePhysicsBody(b2BodyDef def);
    b2PolygonShape CreatePhysicsShape(float width, float height);
    b2Fixture* CreatePhysicsFixture(b2Body* body, b2PolygonShape shape, float density = 0.0f);
    
    bool isInitialized();
    static PhysicsEngine* GetInstance();
    
    ContactHandler* GetContactHandler();
protected:
private:
    bool isInit;
    b2World* theWorld;
    b2Vec2 gravity;
    ContactHandler* handler;
    
    std::vector<b2Body*>* bodies;
    
    PhysicsEngine();
    
    static PhysicsEngine* instance;
};

#endif /* PhysicsEngine_hpp */
