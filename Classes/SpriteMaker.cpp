#include "SpriteMaker.h"

cocos2d::Sprite* SpriteMaker::createSprite(SpriteList sprite)
{
	std::string file_name = "images/ball.png";
	float origin_x = 0;
	float origin_y = 0;
	float width = 0;
	float height = 0;

	switch (sprite)
	{
		case SpriteList::BALL:
			file_name = "images/stone.png";
			width = SPRITE_WIDTH;
			height = SPRITE_HEIGHT;
			break;
		case SpriteList::TREE1:
			file_name = "images/tree1.png";
			width = SPRITE_WIDTH;
			height = SPRITE_HEIGHT;
			break;
		case SpriteList::TREE2:
			file_name = "images/tree2.png";
			width = SPRITE_WIDTH;
			height = SPRITE_HEIGHT;
			break;
		case SpriteList::TREE3:
			file_name = "images/tree3.png";
			width = SPRITE_WIDTH;
			height = SPRITE_HEIGHT;
			break;
		case SpriteList::PADDLE:
			file_name = "images/paddle.png";
			width = SPRITE_WIDTH * 5;
			height = SPRITE_HEIGHT;
			break;
		case SpriteList::GRASS:
			file_name = "images/grass.png";
			width = SPRITE_WIDTH;
			height = SPRITE_HEIGHT;
			break;
		case SpriteList::BLOCK1:
			file_name = "images/block1.png";
			width = SPRITE_WIDTH;
			height = SPRITE_HEIGHT;
			break;
		case SpriteList::BLOCK2:
			file_name = "images/block2.png";
			width = SPRITE_WIDTH;
			height = SPRITE_HEIGHT;
			break;
		case SpriteList::BLOCK3:
			file_name = "images/block3.png";
			width = SPRITE_WIDTH;
			height = SPRITE_HEIGHT;
			break;
		case SpriteList::BLOCK_BOSS:
			file_name = "images/block_boss.png";
			width = SPRITE_WIDTH * 2;
			height = SPRITE_HEIGHT * 2;
			break;
		case SpriteList::BLOCK_TREASURE:
			file_name = "images/block_treasure.png";
			width = SPRITE_WIDTH;
			height = SPRITE_HEIGHT;
			break;
		case SpriteList::MOUNTAIN1:
			file_name = "images/mountain.png";
			width = SPRITE_WIDTH;
			height = SPRITE_HEIGHT;
			break;
		case SpriteList::MOUNTAIN2:
			file_name = "images/mountain.png";
			origin_x = SPRITE_WIDTH;
			width = SPRITE_WIDTH;
			height = SPRITE_HEIGHT;
			break;
	}

	cocos2d::Sprite* mySprite = Sprite::create(file_name, Rect(origin_x, origin_y, width, height));
	
	return mySprite;
}

SpriteMaker::SpriteMaker()
{
}


SpriteMaker::~SpriteMaker()
{
}
