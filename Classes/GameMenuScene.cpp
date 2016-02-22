#include "GameMenuScene.h"
#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"
#include "iostream"

USING_NS_CC;

Scene* GameMenu::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameMenu::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameMenu::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithTTF("Welcome To iBreaker", "fonts/Marker Felt.ttf", 24);
	label->setColor(Color3B(0, 0, 0));
	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height - 10));

	// add the label as a child to this layer
	this->addChild(label, 1);

	auto start_label = Label::createWithTTF("START GAME", "fonts/Marker Felt.ttf", 24);
	start_label->setColor(Color3B(0, 0, 0));
	auto start_button = ui::Button::create("button_normal.png", "button_clicked.png", "button_disabled.png");
	//button->setOpacity(0.5f);
	start_button->addChild(start_label);
	start_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 5 * 4 - 50));
	start_label->setPosition(Vec2(start_button->getLayoutSize().width / 2, start_button->getLayoutSize().height / 2));
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

	auto quit_label = Label::createWithTTF("QUIT", "fonts/Marker Felt.ttf", 24);
	quit_label->setColor(Color3B(0, 0, 0));
	auto quit_button = ui::Button::create("button_normal.png", "button_clicked.png", "button_disabled.png");
	quit_button->addChild(quit_label);
	quit_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 5 * 2 - 50));
	quit_label->setPosition(Vec2(quit_button->getLayoutSize().width / 2, quit_button->getLayoutSize().height / 2));
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

void GameMenu::drawBackground()
{

	Vec2 ground = {0, 0};
	Vec2 tree1 = {24, 0};
	Vec2 tree2 = {48, 0};
	Vec2 tree3 = {72, 0};

	Size visibleSize = Director::getInstance()->getVisibleSize();

	for (int i = 0; i < visibleSize.width / SPRITE_WIDTH; i++)
	{
		for (int j = 0; j < visibleSize.height / SPRITE_HEIGHT; j++)
		{

			if (j == 0 || j == visibleSize.height / SPRITE_HEIGHT - 1)
			{
				auto s = Sprite::create("tree1.png", Rect(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT));
				s->setPosition(Vec2(i * SPRITE_WIDTH + SPRITE_WIDTH / 2, j * SPRITE_HEIGHT + SPRITE_HEIGHT / 2));
				this->addChild(s, -1);
			}
			else if (i == 0)
			{
				auto s = Sprite::create("tree2.png", Rect(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT));
				s->setPosition(Vec2(i * SPRITE_WIDTH + SPRITE_WIDTH / 2, j * SPRITE_HEIGHT + SPRITE_HEIGHT / 2));
				this->addChild(s, -1);
			}
			else if (i == visibleSize.width / SPRITE_WIDTH - 1)
			{
				auto s = Sprite::create("tree3.png", Rect(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT));
				s->setPosition(Vec2(i * SPRITE_WIDTH + SPRITE_WIDTH / 2, j * SPRITE_HEIGHT + SPRITE_HEIGHT / 2));
				this->addChild(s, -1);
			}
			
			else
			{
				auto s = Sprite::create("grasstile.png", Rect(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT));
				s->setPosition(Vec2(i * SPRITE_WIDTH + SPRITE_WIDTH / 2, j * SPRITE_HEIGHT + SPRITE_HEIGHT / 2));
				this->addChild(s, -1);
			}

		}
	}
}

void GameMenu::createSprite(std::string file, int origin_x, int origin_y, int x, int y, int z)
{
	auto mySprite = Sprite::create(file, Rect(origin_x, origin_y, SPRITE_WIDTH, SPRITE_HEIGHT));
	mySprite->setAnchorPoint(Vec2(0, 0));
	mySprite->setPosition(Vec2(x, y));
	this->addChild(mySprite, z);
}

void GameMenu::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

GameMenu::GameMenu(){

}

GameMenu::~GameMenu(){

}

