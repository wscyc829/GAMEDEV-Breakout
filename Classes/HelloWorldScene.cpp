#include "HelloWorldScene.h"
#include "iostream"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
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
	this->addChild(B2DebugDrawLayer::create(world, SCALE_RATIO), 99);

	powerMultiplier = 10; // The value of force is 10
	ball = Sprite::create("ball.png");
	ball->setPosition(Point(500, 500));
	this->addChild(ball);

	paddle = Sprite::create("paddle.png");
	paddle->setPosition(Point(500, 10));
	this->addChild(paddle);

	// Build the frame around three sides of the screen 
	addWall(visibleSize.width, 10, (visibleSize.width / 2), visibleSize.height);// up
	//addWall(visibleSize.width, 10, (visibleSize.width / 2) + 10, 0);// Floor
	addWall(10, visibleSize.height, 0, (visibleSize.height / 2)); // Left
	addWall(10, visibleSize.height, visibleSize.width, (visibleSize.height / 2)); // Right

	for (int i = 1; i <= 31; i++){
		//points[i] = Sprite::create("dot.png");

		//this->addChild(points[i]);
	}

	auto listener = EventListenerMouse::create();
	listener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
	listener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	isStart = false;
	defineBall();
	definePaddle();
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
	
	bodyDef.position.Set(500 / SCALE_RATIO,500 / SCALE_RATIO);
	
	ballBody = world->CreateBody(&bodyDef);
	ballBody->CreateFixture(&fixtureDef);
	ballBody->ApplyLinearImpulse(b2Vec2(10.0f, 10.0f), ballBody->GetWorldCenter(), false);

}

void HelloWorld::definePaddle(){
	b2Body *paddleBody;
	b2BodyDef bodyDef;
	b2FixtureDef fixtureDef; 
	b2PolygonShape bodyShape; 

	log("size:  %f : %f", paddle->getContentSize().width, paddle->getContentSize().height);
	log("scale:  %f : %f", paddle->getContentSize().width / SCALE_RATIO, paddle->getContentSize().height / SCALE_RATIO);
	bodyShape.SetAsBox(paddle->getContentSize().width / SCALE_RATIO, paddle->getContentSize().height / SCALE_RATIO);
	log("l: %d", bodyShape.GetVertexCount());
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