#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "B2DebugDrawLayer.h"
#include "MyContactListener.h"

#include "SpriteMaker.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
	int SPRITE_WIDTH = SpriteMaker::SPRITE_WIDTH;
	int SPRITE_HEIGHT = SpriteMaker::SPRITE_HEIGHT;

	int BOSS_MAX_HP = 100;
	int BLOCK_POINT = 1;
	int BLOCK_BOSS_POINT = 100;
	int BLOCK_TRESSURE_POINT = 10;
	int PLAYER_MAX_HP = 3;

	int score = 0;
	int high_score = 0;

	int boss_hp;
	int player_hp;

	bool isStart;
	bool isReturn;

	Label* score_label;
	Label* high_score_label;

	Sprite* ball;
	Sprite* paddle;

	b2World *world; // World with physic

	MyContactListener *_contactListener;

public:

    static cocos2d::Scene* createScene();
    virtual bool init();
	
	void drawBackground();
	void addWall(float w, float h, float px, float py); // Create the Wall edge around the screen for the ball to collide with.
	void defineBall(); // Create the ball follow Box2D
	void definePaddle();
	void addBlock();

	void updateScore();
	void update(float dt); // Update scene by time
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
	
	void onMouseMove(Event *event);
	void onMouseUp(Event *event);

	void returnToMenu(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
