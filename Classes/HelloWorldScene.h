#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "B2DebugDrawLayer.h"
USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
	bool isStart;
	Sprite* ball;
	Sprite* paddle;

	float powerMultiplier; // Force

	b2World *world; // World with physic
	
	float deltaTime; // The variable to calculate time

	void addWall(float w, float h, float px, float py); // Create the Wall edge around the screen for the ball to collide with.

	void update(float dt); // Update scene by time
	void defineBall(); // Create the ball follow Box2D
	void definePaddle(); 

	void onMouseMove(Event *event);
	void onMouseUp(Event *event);
	bool onContactBegin(PhysicsContact& contact);
    static cocos2d::Scene* createScene();
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
