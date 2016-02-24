#pragma once

#include "cocos2d.h"

USING_NS_CC;

enum SpriteList
{
	BALL, TREE1, TREE2, TREE3, PADDLE, GRASS, BLOCK1, BLOCK2, BLOCK3, 
	BLOCK_BOSS, BLOCK_TRESSURE, MOUNTAIN1, MOUNTAIN2
};

class SpriteMaker
{
public:
	static const int SPRITE_WIDTH = 24;
	static const int SPRITE_HEIGHT = 24;

	static cocos2d::Sprite* createSprite(SpriteList sprite);

	SpriteMaker();
	~SpriteMaker();
};

