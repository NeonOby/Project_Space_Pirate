#include "GameScene.h"
#include "irrKlang.h"
#include "Box2D/Box2D.h"
#include "Physic/B2DebugDrawLayer.h"
#include "Manuel/SoundManager.h"

#include "MouseListener.h"

#include "Level/LevelMap.h"

#include "conio.h"

#include "Konstanten.h"

USING_NS_CC;
using namespace irrklang;

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
	falling = false;
	climbingLeft = false;
	climbingRight = false;
	hasJumped = false;
	jumping = false;
	fallTime = 0;
	waitTime = 0;
	walkDirection = 1;
	jumpStart = 0;
	holdingRight = false;
	holdingLeft = false;
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


	// Define the gravity vector.
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);//No gravity
 
	// create a world object, which will hold and simulate the rigid bodies.
	_world = new b2World(gravity);
	//World is generated, now add some things
	myContactListenerInstance = MyContactListener();
	_world->SetContactListener(&myContactListenerInstance);


	LevelMap map = LevelMap(_world);
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

	_ball = Sprite::create("Pirate.PNG");
	_ball->setPosition(Point(150,300));
	level->addChild(_ball);

	//Player BOdy !!

	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position.Set(100/PTM_RATIO, 300/PTM_RATIO);
	ballBodyDef.userData = _ball;
	_Player = _world->CreateBody(&ballBodyDef);

	b2CircleShape circle;
	circle.m_radius = 16.0/PTM_RATIO;
	circle.m_p = b2Vec2(0,16.0f/PTM_RATIO);
	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &circle;
	ballShapeDef.density = PLAYER_DENITY;
	ballShapeDef.friction = 0.4f;
	ballShapeDef.restitution = 0.0f; //Bounce
	_Player->CreateFixture(&ballShapeDef);

	b2PolygonShape shapeDef2;
	b2Vec2 position;
	position.Set(0.0f, (16.0f + 56.0f)/PTM_RATIO);//No gravity
	shapeDef2.SetAsBox(16/PTM_RATIO, 56/PTM_RATIO, position, 0);

	b2FixtureDef ballShapeDef2;
	ballShapeDef2.shape = &shapeDef2;
	ballShapeDef2.density = PLAYER_DENITY;
	ballShapeDef2.friction = 0.0f;
	ballShapeDef2.restitution = 0.0f;
	b2Fixture * fixture = _Player->CreateFixture(&ballShapeDef2);


	/*
	//Try Ducken
	_Player->DestroyFixture(fixture);

	position.Set(0.0f, (16.0f + 12.0f)/PTM_RATIO);//No gravity
	shapeDef2.SetAsBox(16/PTM_RATIO, 16/PTM_RATIO, position, 0);

	ballShapeDef2.shape = &shapeDef2;
	ballShapeDef2.density = 10.0f;
	ballShapeDef2.friction = 0.0f;
	ballShapeDef2.restitution = 0.0f;
	fixture = _Player->CreateFixture(&ballShapeDef2);
	*/

	//Try Ende
	_Player->SetFixedRotation(true);

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(16/PTM_RATIO, 8/PTM_RATIO, b2Vec2(0,-16/PTM_RATIO+16.0f/PTM_RATIO), 0);
	
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &polygonShape;
    myFixtureDef.density = 0.0f;
	myFixtureDef.friction = 0.0f;
    myFixtureDef.isSensor = true;
    b2Fixture* footSensorFixture = _Player->CreateFixture(&myFixtureDef);
    footSensorFixture->SetUserData( (void*)PLAYER_FOOD );

	//Start Climbing:

	polygonShape.SetAsBox(2/PTM_RATIO, 8/PTM_RATIO, b2Vec2(16/PTM_RATIO,96/PTM_RATIO+16.0f/PTM_RATIO), 0);

	myFixtureDef.shape = &polygonShape;
    myFixtureDef.density = 0.0f;
	myFixtureDef.friction = 0.0f;
    myFixtureDef.isSensor = true;
    b2Fixture* rightSideStartSensorFixture = _Player->CreateFixture(&myFixtureDef);
    rightSideStartSensorFixture->SetUserData( (void*)PLAYER_RIGHT_START_CLIMB );

	polygonShape.SetAsBox(2/PTM_RATIO, 8/PTM_RATIO, b2Vec2(-16/PTM_RATIO,96/PTM_RATIO+16.0f/PTM_RATIO), 0);

	myFixtureDef.shape = &polygonShape;
    myFixtureDef.density = 0.0f;
	myFixtureDef.friction = 0.0f;
    myFixtureDef.isSensor = true;
    b2Fixture* leftSideStartSensorFixture = _Player->CreateFixture(&myFixtureDef);
    leftSideStartSensorFixture->SetUserData( (void*)PLAYER_LEFT_START_CLIMB );

	//Continuing Climbing

	polygonShape.SetAsBox(8/PTM_RATIO, 56/PTM_RATIO, b2Vec2(16/PTM_RATIO,32/PTM_RATIO+16.0f/PTM_RATIO), 0);

	myFixtureDef.shape = &polygonShape;
    myFixtureDef.density = 0.0f;
	myFixtureDef.friction = 0.0f;
    myFixtureDef.isSensor = true;
    b2Fixture* rightSideSensorFixture = _Player->CreateFixture(&myFixtureDef);
    rightSideSensorFixture->SetUserData( (void*)PLAYER_RIGHT_SIDE );

	polygonShape.SetAsBox(8/PTM_RATIO, 56/PTM_RATIO, b2Vec2(-16/PTM_RATIO,32/PTM_RATIO+16.0f/PTM_RATIO), 0);

	myFixtureDef.shape = &polygonShape;
    myFixtureDef.density = 0.0f;
	myFixtureDef.friction = 0.0f;
    myFixtureDef.isSensor = true;
    b2Fixture* leftSideSensorFixture = _Player->CreateFixture(&myFixtureDef);
    leftSideSensorFixture->SetUserData( (void*)PLAYER_LEFT_SIDE );

	//Player END

	log("Player Masse: %f", _Player->GetMass());

	//Floor Body only (no Sprite yet)
	b2Body *_body2;

	b2BodyDef ballBodyDef2;
	//ballBodyDef2.type = b2_staticBody;
	ballBodyDef2.position.Set(0/PTM_RATIO, 0/PTM_RATIO);
	_body2 = _world->CreateBody(&ballBodyDef2);

	b2PolygonShape shapeDef;
	shapeDef.SetAsBox(10000/PTM_RATIO, 50/PTM_RATIO);
 
	b2FixtureDef ballShapeDef3;
	ballShapeDef3.shape = &shapeDef;
	ballShapeDef3.density = 0.0f;
	ballShapeDef3.friction = 0.5f;
	ballShapeDef3.restitution = 0.0f;
	_body2->CreateFixture(&ballShapeDef3);


	//createPlatform(200,200,128,16);
	//createPlatform(400,400,128,16);
	//createPlatform(600,600,128,16);
	
	//createPlatform(900,100,64,512);

	//createPlatform(1250,100,64,512);


	//createKiste(1500,100,32,32);
	//createKiste(1500,550,32,32);
	//createKiste(1200,600,32,32);
	//createKiste(1500,700,32,32);

	//Its the DEBUG Layer for Box2D which draws Debug Thingies
	//level->addChild(B2DebugDrawLayer::create(_world, PTM_RATIO),999);
	//

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

void GameScene::ShootBullet(){
	Sprite* Player = (Sprite*)_Player->GetUserData();
	b2Body * tmpBody = createBullet((Player->getPositionX()+walkDirection*48),Player->getPositionY()+32,8.0f,8.0f);

	Point tmpPoint = MouseListener().getMousePos();

	float dX = tmpPoint.x-(Player->getPositionX()+walkDirection);
	float dY = tmpPoint.y-Player->getPositionY();

	tmpBody->ApplyForceToCenter(b2Vec2(dX,dY));

}

b2Body * GameScene::createBullet(float x, float y, float width, float height){
	//Floor2 Body only (no Sprite yet)
	b2Body *_body;
	

	b2BodyDef BodyDef;
	BodyDef.type = b2_dynamicBody;
	BodyDef.gravityScale = 0.5f; //Weniger Gravity f�r Bullets
	BodyDef.bullet = true;
	BodyDef.position.Set(x/PTM_RATIO, y/PTM_RATIO);
	_body = _world->CreateBody(&BodyDef);
	

	

	b2PolygonShape shapeDef;
	shapeDef.SetAsBox(width/PTM_RATIO, height/PTM_RATIO);
 
	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &shapeDef;
	ballShapeDef.density = 1.0f;
	ballShapeDef.friction = 0.4f;
	ballShapeDef.restitution = 0.0f;

	b2Fixture* leftSideSensorFixture =_body->CreateFixture(&ballShapeDef);
	leftSideSensorFixture->SetUserData((void*)BULLET);

	return _body;
}

b2Body * GameScene::createPlatform(float x, float y, float width, float height){
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
	ballShapeDef.friction = 0.4f;
	ballShapeDef.restitution = 0.0f;
	_body->CreateFixture(&ballShapeDef);

	return _body;
}

b2Body * GameScene::createKiste(float x, float y, float width, float height){
	Sprite * tmpKiste = Sprite::create("Level/Kiste.PNG");
	float scaleX = (width/tmpKiste->getContentSize().width)*2.0f;
	float scaleY = (height/tmpKiste->getContentSize().height)*2.0f;
	tmpKiste->setScaleX(scaleX);
	tmpKiste->setScaleY(scaleY);
	tmpKiste->setPosition(Point(x,y));
	this->addChild(tmpKiste);

	//Floor2 Body only (no Sprite yet)
	b2Body *_body;

	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position.Set(x/PTM_RATIO, y/PTM_RATIO);
	_body = _world->CreateBody(&ballBodyDef);
	_body->SetUserData(tmpKiste);

	b2PolygonShape shapeDef;
	shapeDef.SetAsBox(width/PTM_RATIO, height/PTM_RATIO);
 
	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &shapeDef;
	ballShapeDef.density = 10.0f;
	ballShapeDef.friction = 2.0f;
	ballShapeDef.restitution = 0.0f;
	_body->CreateFixture(&ballShapeDef);

	return _body;
}

float diffToCenter = 150.0f;


void GameScene::update(float dt){

	if(GetAsyncKeyState(VK_LBUTTON)){
		//ShootBullet();
	}

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
			Sprite *ballData = (Sprite*)b->GetUserData();
			if(ballData){
				ballData->setPosition(Point(b->GetPosition().x * PTM_RATIO,
					b->GetPosition().y * PTM_RATIO));
				ballData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle())); 
			}
        }
	}

	bool canMove = true;
	bool canJump = false;
	bool JUMP_PRESSED = false;

	if(GetAsyncKeyState(VK_SPACE))
		JUMP_PRESSED = true;

	if(!jumping && !hasJumped && myContactListenerInstance.playerFootContacts>0){
		canJump = true;
	}

	float maxSpeed = PLAYER_MAX_SPEED*_Player->GetMass();
	float speed = PLAYER_SPEED_CHANGE*_Player->GetMass();
	if(falling){
		speed *= 0.15f;
		maxSpeed *= 0.4f;
	}else if(hasJumped){
		speed *= 0.5f;
		maxSpeed *= 0.5f;
	}

	//Jump
	if(canJump && JUMP_PRESSED){
		canMove = false;

		jumping = true;
		jumpStart = PLAYER_START_JUMP_TIME;
		if(jumpStart<=0.0f){
			_Player->ApplyLinearImpulse(b2Vec2(0.0f, PLAYER_JUMP_SPEED*_Player->GetMass()), _Player->GetWorldCenter());
			hasJumped = true;
			jumping = false;
		}
	}else if(jumping){
		//W�hrend man springt kann man sich nicht bewegen, LoL
		canMove = false;

		jumpStart -= dt;
		if(jumpStart<0.0f){
			jumpStart=0.0f;
			_Player->ApplyLinearImpulse(b2Vec2(0.0f, PLAYER_JUMP_SPEED*_Player->GetMass()), _Player->GetWorldCenter());
			jumping = false;
			hasJumped = true;
		}

		//Slow him down
		_Player->ApplyLinearImpulse(b2Vec2(-PLAYER_SLOW_ON_JUMP* _Player->GetLinearVelocity().x, _Player->GetLinearVelocity().y), _Player->GetWorldCenter());
	}else{

		//Unter Player ist was
		if(myContactListenerInstance.playerFootContacts>0){
			//Hit the Ground, look if we where falling
			if(falling){
				//Hit the ground for first time after falling
				if(fallTime > FALL_TIME_MAX_WITHOUT_DAMAGE){
					waitTime = fallTime*FALL_STOP_TIME_MULTIPLIER;
					//TODO Fall Damage
				}

				slowTime = fallTime*FALL_DEBUF_TIME_MULTIPLIER;
			}
		}

		//Wenn wir gerade Fallen
		if(_Player->GetLinearVelocity().y < 0){
			falling = true;
			//Wir sind abgesprungen
			if(hasJumped){
				hasJumped = false;
			}
		}else{
			falling = false;
		}
		

		if(waitTime >0){
			waitTime-=dt;
			canMove = false;
		}
		if(slowTime >0){
			slowTime-=dt;
			speed *= FALL_DEBUF_SPEED_MULTIPLIER;
			maxSpeed *= FALL_DEBUF_SPEED_MULTIPLIER;
		}
		if(falling){
			fallTime+=dt;
		}else{
			fallTime = 0;
		}

		//Right site climbing
		if(climbingRight && myContactListenerInstance.playerRightSideContacts>0){
			//Continuing Climbing
			_Player->SetLinearVelocity(b2Vec2(_Player->GetLinearVelocity().x, PLAYER_CLIMBING_SPEED*_Player->GetMass()));
			_Player->ApplyLinearImpulse(b2Vec2(1.0f, 0.0f), _Player->GetWorldCenter());
		}else if(climbingRight){
			//End climbing, add small force
			_Player->ApplyLinearImpulse(b2Vec2(5.0f, 0.0f), _Player->GetWorldCenter());
			climbingRight = false;
			waitTime = 0.5f;
		}
		
		//Left site climbing		
		if(climbingLeft && myContactListenerInstance.playerLeftSideContacts>0){
			//Continuing Climbing
			_Player->SetLinearVelocity(b2Vec2(_Player->GetLinearVelocity().x, PLAYER_CLIMBING_SPEED*_Player->GetMass()));
			_Player->ApplyLinearImpulse(b2Vec2(-1.0f, 0.0f), _Player->GetWorldCenter());
		}else if(climbingLeft){
			//End climbing, add small force
			_Player->ApplyLinearImpulse(b2Vec2(-5.0f, 0.0f), _Player->GetWorldCenter());
			climbingLeft = false;
			waitTime = 0.5f;
		}
	}

	
	

	//Get KeyInput, will be outcoded in InputManager
	if(canMove && !climbingRight && GetAsyncKeyState(VK_RIGHT)){
		walkDirection = 1;
		if(!climbingRight && !holdingRight && myContactListenerInstance.playerRightStartClimbContacts>0){
			holdingRight = true;
		}
		if(holdingLeft){
			holdingLeft = false;
		}
		if(climbingLeft){
			_Player->ApplyLinearImpulse(b2Vec2(5.0f, 0.0f), _Player->GetWorldCenter());
			climbingLeft = false;
		}
		if(_Player->GetLinearVelocity().x < maxSpeed){
			_Player->ApplyLinearImpulse(b2Vec2(speed,0.0f), _Player->GetWorldCenter());
			if(_Player->GetLinearVelocity().x > maxSpeed){
				_Player->SetLinearVelocity(b2Vec2(maxSpeed, _Player->GetLinearVelocity().y));
			}
		}
			
	}else if(canMove && !climbingLeft && GetAsyncKeyState(VK_LEFT)){
		walkDirection = -1;
		if(!climbingLeft && !holdingLeft && myContactListenerInstance.playerLeftStartClimbContacts>0){
			holdingLeft = true;
		}
		if(holdingRight){
			holdingRight = false;
		}
		if(climbingRight){
			_Player->ApplyLinearImpulse(b2Vec2(-5.0f, 0.0f), _Player->GetWorldCenter());
			climbingLeft = false;
		}
		if(_Player->GetLinearVelocity().x > -maxSpeed){
			_Player->ApplyLinearImpulse(b2Vec2(-speed,0.0f), _Player->GetWorldCenter());
			if(_Player->GetLinearVelocity().x < -maxSpeed){
				_Player->SetLinearVelocity(b2Vec2(-maxSpeed, _Player->GetLinearVelocity().y));
			}
		}
	}else if(canMove && !falling && !hasJumped){
		_Player->ApplyLinearImpulse(b2Vec2(-_Player->GetLinearVelocity().x, 0.0f), _Player->GetWorldCenter());
	}

	//Mal sehn ob er Clmibing will
	if(holdingRight || holdingLeft){
		_Player->SetLinearVelocity(b2Vec2(0, 0));
		_Player->SetAwake(false);

		if(JUMP_PRESSED){
			if(holdingLeft)
				climbingLeft=true;
			if(holdingRight)
				climbingRight=true;
			holdingRight = false;
			holdingLeft = false;

			_Player->SetAwake(true);
			_Player->ApplyLinearImpulse(b2Vec2(0.0f, PLAYER_CLIMBING_START_JUMP*_Player->GetMass()), _Player->GetWorldCenter());
			//_Player->SetLinearVelocity(b2Vec2(_Player->GetLinearVelocity().x, PLAYER_CLIMBING_SPEED));
		}
	}

	//Update MainLayer Position to scroll with player:

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite *Player = (Sprite *)_Player->GetUserData();
	Point playerPos = level->convertToWorldSpace(Player->getPosition());

	float playerDiffToCenterX = (visibleSize.width/2-playerPos.x)*0.06f;
	float playerDiffToCenterY = (visibleSize.height/2-playerPos.y)*0.06f;

	//Test if X Position is to far left

	if(playerDiffToCenterX>0 && this->getPositionX()>=0){
		//Stop
		this->setPositionX(0);
	}else{
		this->setPositionX(this->getPositionX()+playerDiffToCenterX);
		parallaxLayer->moveX(dt, playerDiffToCenterX);

		himmel->setPositionX(himmel->getPositionX() - playerDiffToCenterX);
	}


	//Test if Y Position is to low or to high

	if(playerDiffToCenterY>0 && this->getPositionY()>=0){
		//To low
		this->setPositionY(0);
	}else if(playerDiffToCenterY<0 && this->getPositionY()<=-280*(1280/visibleSize.height)){
		//To high
		this->setPositionY(-280*(1280/visibleSize.height));
	}else{
		this->setPositionY(this->getPositionY()+playerDiffToCenterY);
		parallaxLayer->moveY(dt, playerDiffToCenterY);

		himmel->setPositionY(himmel->getPositionY() - playerDiffToCenterY);
	}

	

}

void GameScene::menuCloseCallback(Object* pSender){

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
