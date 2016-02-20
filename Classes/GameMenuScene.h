#pragma once

#include "cocos2d.h"

class GameMenu : public cocos2d::Layer
{
	int SPRITE_WIDTH = 30;
	int SPRITE_HEIGHT = 30;
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void drawBackground();
	void createSprite(std::string file, int origin_x, int origin_y, int x, int y, int z);

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(GameMenu);
	GameMenu();
	~GameMenu();
};