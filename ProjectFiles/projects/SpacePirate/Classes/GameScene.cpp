#include "GameScene.h"
#include "irrKlang.h"
#include "Box2D/Box2D.h"
#include "Physic/B2DebugDrawLayer.h"
#include "Manuel/SoundManager.h"

#include "Level/LevelMap.h"

USING_NS_CC;
using namespace irrklang;

#define PTM_RATIO 64.0

b2World *_world;
//The Body of the Sprite, which is the collision shape if you want so
b2Body *_Player;

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

	//Init Variables:
	jumping = false;
	level = new Layer();
	blocks = new Sprite*[200];
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	//Try to play sound here

	SoundManager::playMusic(COMPLEX);


	himmel = Sprite::create("Level/Jungle/Himmel.PNG");

    // position the sprite on the center of the screen
    himmel->setPosition(Point(himmel->getContentSize().width/2*0.5f, himmel->getContentSize().height/2*0.5f));

    // add the sprite as a child to this layer
    this->addChild(himmel, 0);

	parallaxLayer = ParallaxLayer::create();

	this->addChild(parallaxLayer, 1);

	
	
	for(int i=0; i< 200;i++){
		blocks[i] = Sprite::create("Game/Dirt.PNG");

		blocks[i]->setPosition(Point(blocks[i]->getContentSize().width/2 + i * blocks[i]->getContentSize().width - 100*blocks[i]->getContentSize().width,
									 blocks[i]->getContentSize().height/2));

		level->addChild(blocks[i]);
	}

	this->addChild(level, 2);

	LevelMap map = LevelMap();
	map.SetLayerArray(level,level,level);
	map.SetParallaxLayer(parallaxLayer->getLayer(0),parallaxLayer->getLayer(1),parallaxLayer->getLayer(2));
	if(map.LoadMap("resources/maps/map6.xml")){
			log("loading map finished: success");
	}else{
			log("loading map finished: Error");
	}


	//create World and add something
	//The world, world has Gravity etc.
	
	//The sprite of the ball, it moves, rotates, scales with the shape (_ball is not controlled by physic)
	Sprite *_ball;

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
	_Player = _world->CreateBody(&ballBodyDef);

	b2CircleShape circle;
	circle.m_radius = 16.0/PTM_RATIO;
 
	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &circle;
	ballShapeDef.density = 5.0f;
	ballShapeDef.friction = 0.3f;
	ballShapeDef.restitution = 0.0f;
	_Player->CreateFixture(&ballShapeDef);

	b2PolygonShape shapeDef2;
	b2Vec2 position;
	position.Set(0.0f, (16.0f + 24.0f)/PTM_RATIO);//No gravity
	shapeDef2.SetAsBox(16/PTM_RATIO, 40/PTM_RATIO, position, 0);

	b2FixtureDef ballShapeDef2;
	ballShapeDef2.shape = &shapeDef2;
	ballShapeDef2.density = 10.0f;
	ballShapeDef2.friction = 0.0f;
	ballShapeDef2.restitution = 0.0f;
	_Player->CreateFixture(&ballShapeDef2);

	_Player->SetFixedRotation(true);


	//Floor Body only (no Sprite yet)
	b2Body *_body2;

	b2BodyDef ballBodyDef2;
	//ballBodyDef2.type = b2_staticBody;
	ballBodyDef2.position.Set(0/PTM_RATIO, 0/PTM_RATIO);
	_body2 = _world->CreateBody(&ballBodyDef2);

	b2PolygonShape shapeDef;
	shapeDef.SetAsBox(10000/PTM_RATIO, 100/PTM_RATIO);
 
	b2FixtureDef ballShapeDef3;
	ballShapeDef3.shape = &shapeDef;
	ballShapeDef3.density = 1.0f;
	ballShapeDef3.friction = 0.3f;
	ballShapeDef3.restitution = 0.0f;
	_body2->CreateFixture(&ballShapeDef3);


	createPlatform(200,256,128,16);
	createPlatform(400,456,128,16);
	createPlatform(600,656,128,16);
	
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

	//This follows the Player !!
	//this->runAction(Follow::create(_ball));

    return true;
}

void GameScene::createPlatform(float x, float y, float width, float height){
	//Floor2 Body only (no Sprite yet)
	b2Body *_body;

	b2BodyDef ballBodyDef;
	//ballBodyDef2.type = b2_staticBody;
	ballBodyDef.position.Set(x/PTM_RATIO, y/PTM_RATIO);
	_body = _world->CreateBody(&ballBodyDef);

	b2PolygonShape shapeDef;
	shapeDef.SetAsBox(width/PTM_RATIO, height/PTM_RATIO);
 
	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &shapeDef;
	ballShapeDef.density = 1.0f;
	ballShapeDef.friction = 0.3f;
	ballShapeDef.restitution = 0.0f;
	_body->CreateFixture(&ballShapeDef);
}

float diffToCenter = 150.0f;


void GameScene::update(float dt){

	//Test

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
            Sprite *ballData = (Sprite *)b->GetUserData();
            ballData->setPosition(Point(b->GetPosition().x * PTM_RATIO,
                                    b->GetPosition().y * PTM_RATIO));
            ballData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
        }
	}

	//Jump
	if(!jumping && GetAsyncKeyState(VK_SPACE)){
		_Player->ApplyLinearImpulse(b2Vec2(0.0f, 60.0f), _Player->GetWorldCenter());
		SoundManager::playSound(JUMP);
		jumping = true;
	}else{
		//Get if hes not falling anymore:
		if(_Player->GetLinearVelocity().y == 0){
			jumping = false;
		}
	}

	Sprite *Player = (Sprite *)_Player->GetUserData();
	float maxSpeed = 8.0f;
	float speed = 6.0f;
	if(jumping){
		speed *= 0.5f;
		maxSpeed *= 0.5f;
	}

	//Get KeyInput, will be outcoded in InputManager
	if(GetAsyncKeyState(VK_RIGHT)){
		_Player->SetLinearDamping(0.0f);
		if(_Player->GetLinearVelocity().x < maxSpeed){
			_Player->ApplyLinearImpulse(b2Vec2(speed,0.0f), _Player->GetWorldCenter());
			if(_Player->GetLinearVelocity().x > maxSpeed){
				_Player->SetLinearVelocity(b2Vec2(maxSpeed, _Player->GetLinearVelocity().y));
			}
		}
			
	}else if(GetAsyncKeyState(VK_LEFT)){
		_Player->SetLinearDamping(0.0f);
		if(_Player->GetLinearVelocity().x > -maxSpeed){
			_Player->ApplyLinearImpulse(b2Vec2(-speed,0.0f), _Player->GetWorldCenter());
			if(_Player->GetLinearVelocity().x < -maxSpeed){
				_Player->SetLinearVelocity(b2Vec2(-maxSpeed, _Player->GetLinearVelocity().y));
			}
		}
	}else{
		//No Button drückt, brems !!
		if(!jumping){
			_Player->SetLinearDamping(5.0f);
		}else{
			_Player->SetLinearDamping(0.7f);
		}
	}

	//Update MainLayer Position to scroll with player:

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point playerPos = level->convertToWorldSpace(Player->getPosition());

	float playerDiffToCenterX = (visibleSize.width/2-playerPos.x)*0.06f;
	float playerDiffToCenterY = (visibleSize.height/2-playerPos.y)*0.06f;

	this->setPositionX(this->getPositionX()+playerDiffToCenterX);
	parallaxLayer->move(dt, playerDiffToCenterX);

	himmel->setPositionX(himmel->getPositionX() - playerDiffToCenterX);

	//Selbe für y
	this->setPositionY(this->getPositionY()+playerDiffToCenterY);
	//parallaxLayer->move(dt, playerDiffToCenterX);

	himmel->setPositionY(himmel->getPositionY() - playerDiffToCenterY);

}

void GameScene::menuCloseCallback(Object* pSender){

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
