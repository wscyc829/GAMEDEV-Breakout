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

	int BALL_TAG = 1;
	int PADDLE_TAG = 2;
	int BLOCK_TAG = 3;
	int BLOCK_BOSS_TAG = 4;
	int BLOCK_TREASURE_TAG = 5;

	int BOSS_MAX_HP = 20;
	int BLOCK_POINT = 1;
	int BLOCK_BOSS_POINT = 5;
	int BLOCK_TREASURE_POINT = 10;
	int PLAYER_MAX_HP = 3;
	int SPAWN_TIME = 100;

	float BALL_SPEED_X = 8.0f;
	float BALL_SPEED_Y = 8.0f;

	int score = 0;
	int high_score = 0;
	int spawn_count = 0;

	int boss_hp = BOSS_MAX_HP;
	int player_hp;

	bool isStart;
	bool isReturn;

	Label* score_label;
	Label* high_score_label;
	Label* boss_hp_label;

	Sprite* ball;
	Sprite* paddle;
	Sprite* boss;

	b2World *world; // World with physic

	MyContactListener *_contactListener;

public:

    static cocos2d::Scene* createScene();
    virtual bool init();
	
	void drawBackground();
	void addWall(float w, float h, float px, float py); // Create the Wall edge around the screen for the ball to collide with.
	void defineBall(); // Create the ball follow Box2D
	void definePaddle();
	void defineBoss();
	void spawnEnemies();

	void updateInfo();
	void update(float dt); // Update scene by time
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
	
	void onMouseMove(Event *event);
	void onMouseUp(Event *event);

	void returnToMenu(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
