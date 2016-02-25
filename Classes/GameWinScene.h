#pragma once
#include "cocos2d.h"
#include "SpriteMaker.h"

USING_NS_CC;

class GameWin : public cocos2d::Layer
{
	int SPRITE_WIDTH = SpriteMaker::SPRITE_WIDTH;
	int SPRITE_HEIGHT = SpriteMaker::SPRITE_HEIGHT;
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	void drawBackground();

	// implement the "static create()" method manually
	CREATE_FUNC(GameWin);

	GameWin();
	~GameWin();
};

