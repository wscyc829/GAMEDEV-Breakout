#include "GameWinScene.h"
#include "GameMenuScene.h"
#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"

Scene* GameWin::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameWin::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameWin::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithTTF("You Win!!!", "fonts/Marker Felt.ttf", 48);
	label->setColor(Color3B(0, 0, 0));
	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height - 10));

	// add the label as a child to this layer
	this->addChild(label, 1);

	auto start_button = ui::Button::create("button_normal.png", "button_clicked.png", "button_disabled.png");

	auto start_label = Label::createWithTTF("START NEW GAME", "fonts/Marker Felt.ttf", 24);
	start_label->setColor(Color3B(0, 0, 0));
	start_label->setPosition(Vec2(start_button->getLayoutSize().width / 2, start_button->getLayoutSize().height / 2));

	start_button->addChild(start_label);
	start_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 5 * 4 - 50));
	start_button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		int n = 0;
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			n = 1;
			break;
		default:
			break;
		}
		if (n == 1)
		{
			auto director = Director::getInstance();
			auto scene = HelloWorld::createScene();
			// Transition Fade
			//director->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 255, 255)));
			// FlipX
			//director->replaceScene(TransitionFlipX::create(2, scene));

			// Transition Slide In
			director->replaceScene(TransitionSlideInT::create(1, scene));

			// run
			//director->replaceScene(scene);
		}
	});
	this->addChild(start_button, 1);


	auto quit_button = ui::Button::create("button_normal.png", "button_clicked.png", "button_disabled.png");

	auto quit_label = Label::createWithTTF("QUIT", "fonts/Marker Felt.ttf", 24);
	quit_label->setColor(Color3B(0, 0, 0));
	quit_label->setPosition(Vec2(quit_button->getLayoutSize().width / 2, quit_button->getLayoutSize().height / 2));

	quit_button->addChild(quit_label);
	quit_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 5 * 2 - 50));
	quit_button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		int n = 0;
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			n = 1;
			break;
		default:
			break;
		}
		if (n == 1)
		{
			Director::getInstance()->end();
		}
	});
	this->addChild(quit_button);


	drawBackground();

	return true;
}

void GameWin::drawBackground()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	for (int i = 0; i < visibleSize.width / SPRITE_WIDTH; i++)
	{
		for (int j = 0; j < visibleSize.height / SPRITE_HEIGHT; j++)
		{
			Sprite* s;
			if (j == 0 || j >= visibleSize.height / SPRITE_HEIGHT - 1)
			{
				s = SpriteMaker::createSprite(SpriteList::TREE2);
			}
			else if (i == 0)
			{
				s = SpriteMaker::createSprite(SpriteList::TREE3);

			}
			else if (i >= visibleSize.width / SPRITE_WIDTH - 1)
			{
				s = SpriteMaker::createSprite(SpriteList::TREE1);
			}
			else
			{
				s = SpriteMaker::createSprite(SpriteList::GRASS);
			}
			s->setPosition(Vec2(i * SPRITE_WIDTH + SPRITE_WIDTH / 2,
				j * SPRITE_HEIGHT + SPRITE_HEIGHT / 2));
			this->addChild(s, -1);
		}
	}
}

GameWin::GameWin()
{
}


GameWin::~GameWin()
{
}
