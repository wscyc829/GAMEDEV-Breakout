#pragma once

#include "cocos2d.h"
#include "SpriteMaker.h"
#include "SimpleAudioEngine.h"

class GameMenu : public cocos2d::Layer
{
public:
	int SPRITE_WIDTH = SpriteMaker::SPRITE_WIDTH;
	int SPRITE_HEIGHT = SpriteMaker::SPRITE_HEIGHT;

	static cocos2d::Scene* createScene();

	virtual bool init();

	void drawBackground();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(GameMenu);
	GameMenu();
	~GameMenu();
};