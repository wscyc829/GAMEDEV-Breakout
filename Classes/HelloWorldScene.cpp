﻿#include "HelloWorldScene.h"
#include "iostream"
#include "GameMenuScene.h"

#define SCALE_RATIO 32.0

Scene* HelloWorld::createScene()
{ 
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	auto layer = HelloWorld::create();

    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
	world = new b2World(gravity);
	this->addChild(B2DebugDrawLayer::create(world, SCALE_RATIO), 9999);

	ball = SpriteMaker::createSprite(SpriteList::BALL);
	ball->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	ball->setTag(1);
	this->addChild(ball, 1);

	paddle = SpriteMaker::createSprite(SpriteList::PADDLE);
	paddle->setPosition(Vec2(visibleSize.width / 2, SPRITE_HEIGHT / 2 * 3));
	this->addChild(paddle, 1);

	// Build the frame around three sides of the screen 
	addWall(visibleSize.width, 12, (visibleSize.width / 2), visibleSize.height - 12);// up
	addWall(12, visibleSize.height, 12, (visibleSize.height / 2)); // Left
	addWall(12, visibleSize.height, visibleSize.width - 12, (visibleSize.height / 2)); // Right

	isStart = false;
	isReturn = false;
	defineBall();
	definePaddle();
	addBlock();
	drawBackground();

	auto listener = EventListenerMouse::create();
	listener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
	listener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_contactListener = new MyContactListener();
	world->SetContactListener(_contactListener);

	scheduleUpdate();

	CocosDenshion::SimpleAudioEngine::sharedEngine()->getInstance()->playEffect("sounds/game_start_sound.wav");
    return true;
}

void HelloWorld::drawBackground()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto backToMenu = MenuItemImage::create("arrow_normal.png", "arrow_clicked.png",
			CC_CALLBACK_1(HelloWorld::returnToMenu, this));
	backToMenu->setPosition(Vec2(visibleSize.width - backToMenu->getContentSize().width - 12,
			visibleSize.height - backToMenu->getContentSize().height - 12));

	// create menu, it's an autorelease object
	auto menu = Menu::create(backToMenu, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//score
	score_label = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 24);
	score_label->setColor(Color3B(0, 0, 0));
	score_label->setPosition(Vec2(score_label->getContentSize().width + 12,
			visibleSize.height - score_label->getContentSize().height - 12));
	this->addChild(score_label, 1);

	//high score
	high_score_label = Label::createWithTTF("High Score: 0", "fonts/Marker Felt.ttf", 24);
	high_score_label->setColor(Color3B(0, 0, 0));
	high_score_label->setPosition(Vec2(visibleSize.width - high_score_label->getContentSize().width - 12,
			visibleSize.height - high_score_label->getContentSize().height - 12));
	this->addChild(high_score_label, 1);


	for (int i = 0; i < visibleSize.width / SPRITE_WIDTH; i++)
	{
		for (int j = 0; j < visibleSize.height / SPRITE_HEIGHT; j++)
		{
			Sprite* s;
			if (j == 0)
			{
				if (i % 2 == 0)
				{
					s = SpriteMaker::createSprite(SpriteList::MOUNTAIN1);
				}
				else
				{
					s = SpriteMaker::createSprite(SpriteList::MOUNTAIN2);
				}
			}
			else if (j == visibleSize.height / SPRITE_HEIGHT - 1)
			{
				s = SpriteMaker::createSprite(SpriteList::TREE2);
			}
			else if (i == 0)
			{
				s = SpriteMaker::createSprite(SpriteList::TREE3);
			}
			else if (i == visibleSize.width / SPRITE_WIDTH - 1)
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

void HelloWorld::defineBall(){
	b2Body *ballBody; // Body of the ball
	b2BodyDef bodyDef; // Define the above Body
	b2FixtureDef fixtureDef; // Define some static features: friction, restitution, density, etc.
	b2CircleShape bodyShape; // the shape of body

	bodyShape.m_radius = ball->getContentSize().width / 2 / SCALE_RATIO;

	fixtureDef.density = 1.0;
	fixtureDef.friction = 0.0;
	fixtureDef.restitution = 1.0;
	fixtureDef.shape = &bodyShape;

	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = ball;
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	bodyDef.position.Set(visibleSize.width / 2 / SCALE_RATIO, visibleSize.height / 2 / SCALE_RATIO);
	
	ballBody = world->CreateBody(&bodyDef);
	ballBody->CreateFixture(&fixtureDef);
	ballBody->ApplyLinearImpulse(b2Vec2(10.0f, 10.0f), ballBody->GetWorldCenter(), false);
}

void HelloWorld::definePaddle(){
	b2Body *paddleBody;
	b2BodyDef bodyDef;
	b2FixtureDef fixtureDef; 
	b2PolygonShape bodyShape; 

	bodyShape.SetAsBox(paddle->getContentSize().width / 2 /SCALE_RATIO, 
			paddle->getContentSize().height / 2 / SCALE_RATIO);

	fixtureDef.density = 10;
	fixtureDef.friction = 0.0;
	fixtureDef.restitution = 0.0;
	fixtureDef.shape = &bodyShape;

	bodyDef.type = b2_staticBody;
	bodyDef.userData = paddle;

	bodyDef.position.Set(paddle->getPosition().x / SCALE_RATIO, 
			paddle->getPosition().y / SCALE_RATIO);

	paddleBody = world->CreateBody(&bodyDef);
	paddleBody->CreateFixture(&fixtureDef);
}

void HelloWorld::addWall(float w, float h, float px, float py) {
	b2Body *floorBody;
	b2BodyDef floorBodyDef;
	b2FixtureDef floorFixtureDef;
	b2PolygonShape floorShape; 

	floorShape.SetAsBox(w / SCALE_RATIO, h / SCALE_RATIO); 

	floorFixtureDef.density = 10.0;
	floorFixtureDef.friction = 0.0;
	floorFixtureDef.restitution = 0.0;
	floorFixtureDef.shape = &floorShape;

	floorBodyDef.type = b2_staticBody;
	floorBodyDef.position.Set(px / SCALE_RATIO, py / SCALE_RATIO);
	
	floorBody = world->CreateBody(&floorBodyDef);
	floorBody->CreateFixture(&floorFixtureDef);
}

void HelloWorld::addBlock()
{
	for (int i = 0; i < 10; i++) 
	{

		static int padding = 20;

		Sprite *block = SpriteMaker::createSprite(SpriteList::BLOCK1);
		int xOffset = padding + block->getContentSize().width / 2 +
				((block->getContentSize().width + padding)*i);
		block->setPosition(ccp(xOffset, 250));
		block->setTag(2);
		this->addChild(block);

		// Create block body
		b2BodyDef blockBodyDef;
		blockBodyDef.type = b2_staticBody;
		blockBodyDef.position.Set(xOffset / SCALE_RATIO, 250 / SCALE_RATIO);
		blockBodyDef.userData = block;
		b2Body *blockBody = world->CreateBody(&blockBodyDef);

		// Create block shape
		b2PolygonShape blockShape;
		blockShape.SetAsBox(block->getContentSize().width / SCALE_RATIO / 2,
				block->getContentSize().height / SCALE_RATIO / 2);

		// Create shape definition and add to body
		b2FixtureDef blockShapeDef;
		blockShapeDef.shape = &blockShape;
		blockShapeDef.density = 10.0;
		blockShapeDef.friction = 0.0;
		blockShapeDef.restitution = 0.0f;
		blockBody->CreateFixture(&blockShapeDef);
	}
}

void HelloWorld::update(float dt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	int positionIterations = 10;  // Location
	int velocityIterations = 10; // Velocity

	if (isStart && !isReturn)
	{
		world->Step(dt, velocityIterations, positionIterations);
		// Navigate all bodies of world
		for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext())
		{
			// Consider which body is attached to Sprite
			if (body->GetUserData())
			{
				Sprite* b = static_cast<Sprite*>(body->GetUserData());
				if (b == ball)
				{	
					// Set the position again for this Sprite follow the position of body 
					//( body is gradually falling by time), 
					//remember to multiply with RATIO to convert into pixel
					b->setPosition(Point(body->GetPosition().x * SCALE_RATIO, 
							body->GetPosition().y * SCALE_RATIO));
					b->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
					if (b->getPosition().y <= 0)
					{
						isStart = false;
						world->DestroyBody(body);
						defineBall();
						ball->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
					}
				}
				else if (b == paddle)
				{
					body->SetTransform(b2Vec2(paddle->getPosition().x / SCALE_RATIO, paddle->getPosition().y / SCALE_RATIO), 0);
				}
			}
		}

		std::vector<b2Body *>toDestroy;
		std::vector<MyContact>::iterator pos;
		for (pos = _contactListener->_contacts.begin();
			pos != _contactListener->_contacts.end(); ++pos) 
		{
			MyContact contact = *pos;

			b2Body *bodyA = contact.fixtureA->GetBody();
			b2Body *bodyB = contact.fixtureB->GetBody();
			if (bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL) 
			{
				CCSprite *spriteA = (CCSprite *)bodyA->GetUserData();
				CCSprite *spriteB = (CCSprite *)bodyB->GetUserData();

				// Sprite A = ball, Sprite B = Block
				if (spriteA->getTag() == 1 && spriteB->getTag() == 2) 
				{
					if (std::find(toDestroy.begin(), toDestroy.end(), bodyB)
						== toDestroy.end()) {
						toDestroy.push_back(bodyB);
					}
				}
				// Sprite A = block, Sprite B = ball
				else if (spriteA->getTag() == 2 && spriteB->getTag() == 1) 
				{
					if (std::find(toDestroy.begin(), toDestroy.end(), bodyA)
						== toDestroy.end()) {
						toDestroy.push_back(bodyA);
					}
				}
			}
			else //
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->getInstance()->playEffect("sounds/ball_bouncing.wav");
			}
		}


		std::vector<b2Body *>::iterator pos2;
		for (pos2 = toDestroy.begin(); pos2 != toDestroy.end(); ++pos2) 
		{
			b2Body *body = *pos2;
			if (body->GetUserData() != NULL) 
			{
				CCSprite *sprite = (CCSprite *)body->GetUserData();
				this->removeChild(sprite, true);

				score++;
			}
			world->DestroyBody(body);
		}

		//if (!blockFound)
		//{
		//	GameOverScene *gameOverScene = GameOverScene::node();
		////	gameOverScene->getLayer()->getLabel()->setString("You Win!");
		//	CCDirector::sharedDirector()->replaceScene(gameOverScene);
		//}

		if (toDestroy.size() > 0)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->getInstance()->playEffect("sounds/enemy_killed_sound.wav");
			updateScore();
		}
	}
}

void HelloWorld::updateScore()
{
	std::string s = "Score: ";
	s += std::to_string(score);
	score_label->setString(s);

	if (score > high_score)
	{
		high_score = score;
		s = "High Score: ";
		s += std::to_string(high_score);
		high_score_label->setString(s);
	}
}

void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	world->DrawDebugData();
	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void HelloWorld::onMouseMove(Event *event)
{
	EventMouse* e = (EventMouse*)event;
	float x = e->getCursorX();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	if (x < paddle->getContentSize().width / 2 + SPRITE_WIDTH / 2)
	{
		x = paddle->getContentSize().width / 2 + SPRITE_WIDTH / 2;
	}
	else if (x > visibleSize.width - paddle->getContentSize().width / 2 - SPRITE_WIDTH / 2)
	{
		x = visibleSize.width - paddle->getContentSize().width / 2 - SPRITE_WIDTH / 2;
	}
	paddle->setPositionX(x);
}

void HelloWorld::onMouseUp(Event *event)
{
	EventMouse* e = (EventMouse*)event;
	isStart = true;
}

void HelloWorld::returnToMenu(Ref* pSender)
{
	auto director = Director::getInstance();
	auto scene = GameMenu::createScene();

	isReturn = true;
	// run
	director->replaceScene(TransitionSlideInT::create(1, scene));
}

