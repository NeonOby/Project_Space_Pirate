#include "Player.h"

USING_NS_CC;


//Custom static Create method (For cocos2d)
//Because we need to give player
//! World
//! CurrentSpawn
Player* Player::create(b2World *pWorld, cocos2d::Point *pSpawn)
{
	Player *pRet = new Player(pWorld);
	if (pRet && pRet->init(pSpawn))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	} 
}

/*
: \
	mPlayerBody(NULL),\
	mPlayerSprite(NULL),\
	mJumpCooldownTimer(0.0f),\
	mJumpStartTimer(0.0f),\
	mFallTimer(0.0f),\
	mSlowTimer(0.0f),\
	mWaitTimer(0.0f),\
	mDirection(1),\
	mGrounded(false),\
	mHoldingLeft(false),\
	mHoldingRight(false),\
	mClimbingLeft(false),\
	mClimbingRight(false)
*/

Player::Player(b2World *pWorld): \
	mPlayerBody(NULL),\
	mPlayerSprite(NULL),\
	mJumpCooldownTimer(0.0f),\
	mJumpStartTimer(0.0f),\
	mFallTimer(0.0f),\
	mSlowTimer(0.0f),\
	mWaitTimer(0.0f),\
	mDirection(1),\
	mGrounded(false),\
	mHoldingLeft(false),\
	mHoldingRight(false),\
	mClimbingLeft(false),\
	mClimbingRight(false){

	mWorld = pWorld;
}

Player::~Player(){
	//! We don't need to delete anything

	//! World gets deleted by Box2D
	mWorld = NULL;
	//! Body gets deleted by Box2D
	mPlayerBody = NULL;
	//! Sprite is autorelease (CoCos2D-X)
	mPlayerSprite = NULL;
}

bool Player::init(Point *pSpawn){
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	//If b2World is NULL we can't create a b2Body
	if(!mWorld){
		log("Couldn't create Player");
		return false;
	}
	
	//! Cache Animations
	CacheAnimations();

	//! Create Sprite/His Look
	CreateSprite();

	//! Create Body
	CreateBody(pSpawn);

	//! Create Sensors
	CreateSensors();

	return true;
}

void Player::update(float dt){
	//! Here will be all Logic we can do
	//! I think the Player Object should know
	//! what he wants to do :D

	//! Just like the Player itself
}

void Player::CacheAnimations(){
	//! Try to load all Frames into Cached Animations
	//! This needs some time, But I can't write the Code without
	//! Knowing how much frames, animations we have etc.
}

void Player::CreateSprite(){
	//! Later we take the Sprite from AnimationFrameCache
	//! Because why should we load it 2 times
	mPlayerSprite = Sprite::create("Pirate.PNG");
}

void Player::CreateBody(Point *pSpawn){

	//! Variables
	b2BodyDef tmpBodyDef;

	b2CircleShape tmpCircleShape;
	b2FixtureDef tmpFixtureDef;

	b2PolygonShape tmpPolygonShape;
	//! EndOf Variables

	//This equals for every Fixture
	tmpFixtureDef.density = PLAYER_DENITY;

	//! Create the Body and add it to the World
	//Dynamic Bodies are effected by gravity and other forces
	tmpBodyDef.type = b2_dynamicBody;
	tmpBodyDef.position.Set(pSpawn->x/PTM_RATIO, pSpawn->y/PTM_RATIO);
	//UserData for b2Bodies are always Sprites
	//See GameScene update for more information why we need this
	tmpBodyDef.userData = mPlayerSprite;

	//This actually creates the b2Body and adds it to the World
	mPlayerBody = mWorld->CreateBody(&tmpBodyDef);
	//Body should not rotate, that would suck
	mPlayerBody->SetFixedRotation(true);
	//! EndOf Body

	//! Create Box for uper Body
	tmpPolygonShape.SetAsBox(16/PTM_RATIO, 56/PTM_RATIO, b2Vec2(0.0f, (16.0f + 56.0f)/PTM_RATIO), 0);
	
	tmpFixtureDef.shape = &tmpPolygonShape;
	mPlayerBody->CreateFixture(&tmpFixtureDef);
	//! EndOf Box for uper Body

	//! Create Circle for Feet
	tmpCircleShape.m_radius = 16.0/PTM_RATIO;
	tmpCircleShape.m_p = b2Vec2(0,16.0f/PTM_RATIO);
	
	tmpFixtureDef.shape = &tmpCircleShape;
	tmpFixtureDef.friction = 0.08f;
	mPlayerBody->CreateFixture(&tmpFixtureDef);
	//! EndOf Circle for Feet
}

void Player::CreateSensors(){

	//We use this variables for all sensors
	//? less memory and time consumption
	b2PolygonShape tmpPolygonShape;
	b2FixtureDef tmpFixtureDef;

	//We set this one time, should work
	tmpFixtureDef.shape = &tmpPolygonShape;
	tmpFixtureDef.isSensor = true;

	//! Create Food Sensor
	//? Width  : same as player collider (16 Pixel)
	//? Height : Not to height (8 Pixel)

	tmpPolygonShape.SetAsBox(16/PTM_RATIO, 8/PTM_RATIO, b2Vec2(0,-16/PTM_RATIO+16.0f/PTM_RATIO), 0);
	mPlayerBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)PLAYER_FOOT );

	//! EndOf Food Sensor


	//! Create Start Climbing Sensor
	//? Should be quite small

	//! Right
	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 8/PTM_RATIO, b2Vec2(16/PTM_RATIO,96/PTM_RATIO), 0);
	mPlayerBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)PLAYER_RIGHT_START_CLIMB );

	//! Left
	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 8/PTM_RATIO, b2Vec2(-16/PTM_RATIO,96/PTM_RATIO), 0);
	mPlayerBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)PLAYER_LEFT_START_CLIMB );

	//! EndOf Start Climbing Sensor


	//! Create Climbing Sensor
	//? Goes all the way from "Start Sensor" to the feet
	//? As long as this sensor gets collision, the player is lifting himself

	//! Right
	tmpPolygonShape.SetAsBox(8/PTM_RATIO, 56/PTM_RATIO, b2Vec2(16/PTM_RATIO,32/PTM_RATIO+16.0f/PTM_RATIO), 0);
	mPlayerBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)PLAYER_RIGHT_SIDE );

	//! Left
	tmpPolygonShape.SetAsBox(8/PTM_RATIO, 56/PTM_RATIO, b2Vec2(-16/PTM_RATIO,32/PTM_RATIO+16.0f/PTM_RATIO), 0);
	mPlayerBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)PLAYER_LEFT_SIDE );

	//! EndOf Climbing Sensor
}

/* Old Player
void l(){

	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position.Set(PlayerSpawn.x/PTM_RATIO, PlayerSpawn.y/PTM_RATIO);
	ballBodyDef.userData = Player;
	_Player = _world->CreateBody(&ballBodyDef);

	b2CircleShape circle;
	circle.m_radius = 16.0/PTM_RATIO;
	circle.m_p = b2Vec2(0,16.0f/PTM_RATIO);
	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &circle;
	ballShapeDef.density = PLAYER_DENITY;
	ballShapeDef.friction = 0.08f;
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

	s
	
	//Try Ducken
	_Player->DestroyFixture(fixture);

	position.Set(0.0f, (16.0f + 12.0f)/PTM_RATIO);//No gravity
	shapeDef2.SetAsBox(16/PTM_RATIO, 16/PTM_RATIO, position, 0);

	ballShapeDef2.shape = &shapeDef2;
	ballShapeDef2.density = 10.0f;
	ballShapeDef2.friction = 0.0f;
	ballShapeDef2.restitution = 0.0f;
	fixture = _Player->CreateFixture(&ballShapeDef2);
	

	//Try Ende
	_Player->SetFixedRotation(true);

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(14/PTM_RATIO, 8/PTM_RATIO, b2Vec2(0,-16/PTM_RATIO+16.0f/PTM_RATIO), 0);
	
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &polygonShape;
	myFixtureDef.density = 0.0f;
	myFixtureDef.friction = 0.0f;
	myFixtureDef.isSensor = true;
	b2Fixture* footSensorFixture = _Player->CreateFixture(&myFixtureDef);
	footSensorFixture->SetUserData( (void*)PLAYER_FOOD );

	//Start Climbing:

	polygonShape.SetAsBox(2/PTM_RATIO, 8/PTM_RATIO, b2Vec2(16/PTM_RATIO,96/PTM_RATIO), 0);

	myFixtureDef.shape = &polygonShape;
	myFixtureDef.density = 0.0f;
	myFixtureDef.friction = 0.0f;
	myFixtureDef.isSensor = true;
	b2Fixture* rightSideStartSensorFixture = _Player->CreateFixture(&myFixtureDef);
	rightSideStartSensorFixture->SetUserData( (void*)PLAYER_RIGHT_START_CLIMB );

	polygonShape.SetAsBox(2/PTM_RATIO, 8/PTM_RATIO, b2Vec2(-16/PTM_RATIO,96/PTM_RATIO), 0);

	myFixtureDef.shape = &polygonShape;
	myFixtureDef.density = 0.0f;
	myFixtureDef.friction = 0.0f;
	myFixtureDef.isSensor = true;
	b2Fixture* leftSideStartSensorFixture = _Player->CreateFixture(&myFixtureDef);
	leftSideStartSensorFixture->SetUserData( (void*)PLAYER_LEFT_START_CLIMB );

	// Continuing Climbing

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
	}
*/