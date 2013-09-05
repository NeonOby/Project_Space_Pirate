#include "GameScene.h"
#include "irrKlang.h"
#include "Box2D/Box2D.h"
#include "Physic/B2DebugDrawLayer.h"
#include "Manuel/SoundManager.h"

USING_NS_CC;
using namespace irrklang;

#define PTM_RATIO 64.0

b2World *_world;

Scene* GameScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	//Try to play sound here

	Sprite* sprite = Sprite::create("Level/Jungle/Himmel.PNG");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(sprite->getContentSize().width/2*0.5f, sprite->getContentSize().height/2*0.5f));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

	parallaxLayer = ParallaxLayer::create();

	this->addChild(parallaxLayer, 1);

	level = new Layer();

	blocks = new Sprite*[200];

	for(int i=0; i< 200;i++){
		blocks[i] = Sprite::create("Game/Dirt.PNG");

		blocks[i]->setPosition(Point(blocks[i]->getContentSize().width/2 + i * blocks[i]->getContentSize().width - 100*blocks[i]->getContentSize().width,
									 blocks[i]->getContentSize().height/2));

		level->addChild(blocks[i]);
	}

	this->addChild(level, 2);

	//create World and add something


	//The world, world has Gravity etc.
	
	//The Body of the Sprite, which is the collision shape if you want so
	b2Body *_body;
	//The sprite of the ball, it moves, rotates, scales with the shape (_ball is not controlled by physic)
	CCSprite *_ball;

	_ball = Sprite::create("Game/Dirt.PNG");
	_ball->setPosition(Point(200,200));
	level->addChild(_ball);

	// Define the gravity vector.
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.0f);//No gravity
 
	// create a world object, which will hold and simulate the rigid bodies.
	_world = new b2World(gravity);
	//World is generated, now add some things


	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position.Set(100/PTM_RATIO, 300/PTM_RATIO);
	ballBodyDef.userData = _ball;
	_body = _world->CreateBody(&ballBodyDef);

	b2CircleShape circle;
	circle.m_radius = 26.0/PTM_RATIO;
 
	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &circle;
	ballShapeDef.density = 1.0f;
	ballShapeDef.friction = 0.2f;
	ballShapeDef.restitution = 0.0f;
	_body->CreateFixture(&ballShapeDef);


	//Floor Body only (no Sprite yet)
	b2Body *_body2;

	b2BodyDef ballBodyDef2;
	//ballBodyDef2.type = b2_staticBody;
	ballBodyDef2.position.Set(0/PTM_RATIO, 0/PTM_RATIO);
	_body2 = _world->CreateBody(&ballBodyDef2);

	b2PolygonShape shapeDef;
	shapeDef.SetAsBox(1080/PTM_RATIO, 100/PTM_RATIO);
 
	b2FixtureDef ballShapeDef2;
	ballShapeDef2.shape = &shapeDef;
	ballShapeDef2.density = 1.0f;
	ballShapeDef2.friction = 0.2f;
	ballShapeDef2.restitution = 0.0f;
	_body2->CreateFixture(&ballShapeDef2);
 
	
	//Its the DEBUG Layer for Box2D which draws Debug Thingies
	level->addChild(B2DebugDrawLayer::create(_world, PTM_RATIO),999);


    // add a "close" icon to exit the progress. it's an autorelease object
    MenuItemImage *closeItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(GameScene::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    Menu* menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 5);

    
    LabelTTF* label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

	this->schedule(schedule_selector(GameScene::update), 0.005F);

    return true;
}



void GameScene::update(float dt){

	//Update World, Box2D updates position, rotation, calculates things etc.
	_world->Step(dt, 10, 10);

	//Update Position of every Sprite to its b2Body Referenz
	//Why?
	/*
	Box2D does NOT update Position of CocosSprites,
	it does only update Position of its own Bodys (b2Body),
	which are present in the world

	That means, that we have to update Position, Rotation
	and if its changed by Box2D scale of sprites
	*/
    for(b2Body *b = _world->GetBodyList(); b; b=b->GetNext()) {
        if (b->GetUserData() != NULL) {
            CCSprite *ballData = (CCSprite *)b->GetUserData();
            ballData->setPosition(Point(b->GetPosition().x * PTM_RATIO,
                                    b->GetPosition().y * PTM_RATIO));
            ballData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
        }        
    }

	//Get KeyInput, will be outcoded in InputManager
	if(GetAsyncKeyState(VK_RIGHT)){
		parallaxLayer->move(dt, -1);
		level->setPositionX(level->getPositionX()-dt*speed);
	}else if(GetAsyncKeyState(VK_LEFT)){
		parallaxLayer->move(dt, 1);
		level->setPositionX(level->getPositionX()+dt*speed);
	}

}

void GameScene::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
