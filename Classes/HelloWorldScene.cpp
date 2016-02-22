#include "HelloWorldScene.h"
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

	auto closeItem = MenuItemImage::create(
		"arrow_normal.png",
		"arrow_clicked.png",
		CC_CALLBACK_1(HelloWorld::returnToMenu, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width,
		origin.y + visibleSize.height - closeItem->getContentSize().height));
	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	world = new b2World(gravity);
	this->addChild(B2DebugDrawLayer::create(world, SCALE_RATIO), 9999);

	drawBackground();

	ball = Sprite::create("ball.png");
	ball->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(ball);

	paddle = Sprite::create("paddle.png");
	paddle->setPosition(Point(visibleSize.width / 2, paddle->getContentSize().height / 2 + 5));
	this->addChild(paddle);

	// Build the frame around three sides of the screen 
	addWall(visibleSize.width, 12, (visibleSize.width / 2), visibleSize.height - 12);// up
	//addWall(visibleSize.width, 10, (visibleSize.width / 2) + 10, 0);// Floor
	addWall(12, visibleSize.height, 12, (visibleSize.height / 2)); // Left
	addWall(12, visibleSize.height, visibleSize.width - 12, (visibleSize.height / 2)); // Right

	isStart = false;
	defineBall();
	definePaddle();

	auto listener = EventListenerMouse::create();
	listener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
	listener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	scheduleUpdate();

    return true;
}

void HelloWorld::defineBall(){
	b2Body *ballBody; // Body of the ball
	b2BodyDef bodyDef; // Define the above Body
	b2FixtureDef fixtureDef; // Define some static features: friction, restitution, density, etc.
	b2CircleShape bodyShape; // the shape of body

	bodyShape.m_radius = ball->getContentSize().width / 2 / SCALE_RATIO;

	fixtureDef.density = 1;
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

	bodyShape.SetAsBox(paddle->getContentSize().width / 2 /SCALE_RATIO, paddle->getContentSize().height / 2 / SCALE_RATIO);

	fixtureDef.density = 1;
	fixtureDef.friction = 0.0;
	fixtureDef.restitution = 0.0;
	fixtureDef.shape = &bodyShape;

	bodyDef.type = b2_staticBody;
	bodyDef.userData = paddle;

	bodyDef.position.Set(paddle->getPosition().x / SCALE_RATIO, paddle->getPosition().y / SCALE_RATIO);

	paddleBody = world->CreateBody(&bodyDef);
	paddleBody->CreateFixture(&fixtureDef);
}

void HelloWorld::addWall(float w, float h, float px, float py) {
	b2Body *floorBody;
	b2BodyDef floorBodyDef;
	b2FixtureDef floorFixture;
	b2PolygonShape floorShape; // the floor shape

	floorShape.SetAsBox(w / SCALE_RATIO, h / SCALE_RATIO); // square, or rectangle

	floorFixture.density = 0.0;
	floorFixture.friction = 10;
	floorFixture.restitution = 0.0;
	floorFixture.shape = &floorShape;

	floorBodyDef.type = b2_staticBody;
	floorBodyDef.position.Set(px / SCALE_RATIO, py / SCALE_RATIO);
	
	floorBody = world->CreateBody(&floorBodyDef);
	floorBody->CreateFixture(&floorFixture);
	
}


void HelloWorld::drawBackground()
{
	int SPRITE_WIDTH = 24;
	int SPRITE_HEIGHT = 24;

	Vec2 ground = { 0, 0 };
	Vec2 tree1 = { 24, 0 };
	Vec2 tree2 = { 48, 0 };
	Vec2 tree3 = { 72, 0 };

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
void HelloWorld::update(float dt){
	int positionIterations = 10;  // Location
	int velocityIterations = 10; // Velocity

	deltaTime = dt; // Time step

	// Navigate all bodies of world
	if (isStart)
	{
		world->Step(dt, velocityIterations, positionIterations);
		for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext())
		{
			// Consider which body is attached to Sprite
			if (body->GetUserData())
			{
				
				Sprite* b = static_cast<Sprite*>(body->GetUserData());
				if (b == ball)
				{	// return the ball sprite 
					Sprite *sprite = (Sprite *)body->GetUserData();
					// Set the position again for this Sprite follow the position of body ( body is gradually falling by time), remember to multiply with RATIO to convert into pixel
					sprite->setPosition(Point(body->GetPosition().x * SCALE_RATIO, body->GetPosition().y * SCALE_RATIO));
					
					if (b->getPosition().y <= 0)
					{
						isStart = false;
						world->DestroyBody(body);
						defineBall();
					}
				}
				else
				{
					body->SetTransform(b2Vec2(paddle->getPosition().x / SCALE_RATIO, paddle->getPosition().y / SCALE_RATIO), 0);
				}
			}
		}
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
	paddle->setPositionX(e->getCursorX());
}

void HelloWorld::onMouseUp(Event *event)
{
	EventMouse* e = (EventMouse*)event;
	isStart = true;
}

bool HelloWorld::onContactBegin(PhysicsContact& contact)
{
	// Get two collided object
	auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();

	// Check kinds of objects
	int tagA = spriteA->getTag();
	int tagB = spriteB->getTag();

	if (tagA == 1) // is brick
	{

		//this->removeChild(spriteA, true); // delete brick

		//spriteA->removeFromParentAndCleanup(true);
	}

	if (tagB == 3)  // is brick
	{
		//this->removeChild(spriteB, true); // delete brick

		//spriteB->removeFromParentAndCleanup(true);
	}

	// If the ball collides with the floor and the coordinate Y of the ball is smaller than the paddle, Game Over happens
	if ((tagA == 0 || tagB == 0)& ((ball->getPositionY()) <= 20))
	{
	//	auto gameOverScene = GameOverScene::create();
	//	gameOverScene->getLayer()->getLabel()->setString("You Lose!");
	//	Director::getInstance()->replaceScene(gameOverScene);
	//	log("lose");
	}

	return true;
}

void HelloWorld::returnToMenu(Ref* pSender)
{
	auto director = Director::getInstance();
	auto scene = GameMenu::createScene();

	// run
	director->replaceScene(TransitionSlideInT::create(1, scene));
}