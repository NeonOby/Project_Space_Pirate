#include "Enemy.h"

USING_NS_CC;


//Custom static Create method (For cocos2d)
//Because we need to give enemy
//! World
//! CurrentSpawn
Enemy* Enemy::create(b2World *pWorld, cocos2d::Point *pSpawn, b2Body* pPlayer)
{
	Enemy *pRet = new Enemy(pWorld, pSpawn, pPlayer);
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

Enemy::Enemy(): \
	mEnemyBody(NULL),\
	mEnemySprite(NULL),\
	mJumpCooldownTimer(0.0f),\
	mJumpStartTimer(0.0f),\
	mFallTimer(0.0f),\
	mSlowTimer(0.0f),\
	mWaitTimer(0.0f),\
	mDirection(1),\
	mGrounded(false),\
	mPlayer(NULL),\
	footContacts(NULL)
{
	mWorld = NULL;
}

Enemy::Enemy(b2World *pWorld, cocos2d::Point *pSpawn, b2Body* pPlayer): \
	mEnemyBody(NULL),\
	mEnemySprite(NULL),\
	mJumpCooldownTimer(0.0f),\
	mJumpStartTimer(0.0f),\
	mFallTimer(0.0f),\
	mSlowTimer(0.0f),\
	mWaitTimer(0.0f),\
	mDirection(1),\
	mGrounded(false),\
	footContacts(NULL)
{



	mWorld = pWorld;
	mPlayer = pPlayer;
	//If b2World is NULL we can't create a b2Body
	if(!mWorld){
		log("Couldn't create Enemy");
	}

	//! Cache Animations
	CacheAnimations();

	//! Create Sprite/His Look
	CreateSprite();

	//! Create Body
	CreateBody(pSpawn);

	//! Create Sensors
	CreateSensors();

	footContacts = MyContactListener::GetInstance()->AddListener(mEnemyBody, ENEMY_FOOT);
	log("My Pointer to Contacts %p", footContacts);
}

Enemy::~Enemy(){
	//! We don't need to delete anything

	//! World gets deleted by Box2D
	mWorld = NULL;
	//! Body gets deleted by Box2D
	mEnemyBody = NULL;
	//! Sprite is autorelease (CoCos2D-X)
	mEnemySprite = NULL;
}

bool Enemy::init(Point *pSpawn){
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	return true;
}


void Enemy::update(float dt){
	//! Here will be all Logic we can do
	//! I think the Enemy Object should know
	//! what he wants to do :D
	//log("pos %f %f", mPlayer->GetPosition().x, mPlayer->GetPosition().y);

	b2Vec2 vec = mPlayer->GetPosition();
	vec.operator-=(mEnemyBody->GetPosition());

	float length = vec.Length();
	float lengthInPixls = length * PTM_RATIO;

	vec.x = vec.x / length;
	vec.y = 0;

	vec.operator*=(5);


	//TODO: enemyJump
	if (lengthInPixls < Director::getInstance()->getVisibleSize().width/2)
	{
		mEnemyBody->SetLinearVelocity(vec);
	}
	if(*footContacts>0 && lengthInPixls < 256 && lengthInPixls > 300)
	{
		//mEnemyBody->ApplyForceToCenter(b2Vec2(-(20*(mEnemyBody->GetLinearVelocity().x*PLAYER_SLOW_MULTIPLIER)), 0.0f));
		mEnemyBody->ApplyForceToCenter(b2Vec2(mEnemyBody->GetLinearVelocity().x, 30*mEnemyBody->GetMass()));		
	}
	
	//! Just like the Enemy itself
}

void Enemy::CacheAnimations(){
	//! Try to load all Frames into Cached Animations
	//! This needs some time, But I can't write the Code without
	//! Knowing how much frames, animations we have etc.
}

void Enemy::CreateSprite(){
	//! Later we take the Sprite from AnimationFrameCache
	//! Because why should we load it 2 times
	//mEnemySprite = Sprite::create("melee_left.PNG");
}

void Enemy::CreateBody(Point *pSpawn){

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
	tmpBodyDef.userData = mEnemySprite;

	//This actually creates the b2Body and adds it to the World
	mEnemyBody = mWorld->CreateBody(&tmpBodyDef);
	//Body should not rotate, that would suck
	mEnemyBody->SetFixedRotation(true);
	//! EndOf Body

	//! Create Box for uper Body
	tmpPolygonShape.SetAsBox(16/PTM_RATIO, 56/PTM_RATIO, b2Vec2(0.0f, (16.0f + 56.0f)/PTM_RATIO), 0);

	tmpFixtureDef.shape = &tmpPolygonShape;
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData((void*)ENEMY);
	//! EndOf Box for uper Body

	//! Create Circle for Feet
	tmpCircleShape.m_radius = 16.0/PTM_RATIO;
	tmpCircleShape.m_p = b2Vec2(0,16.0f/PTM_RATIO);

	tmpFixtureDef.shape = &tmpCircleShape;
	tmpFixtureDef.friction = 0.08f;
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData((void*)ENEMY);
	//! EndOf Circle for Feet
}

void Enemy::CreateSensors(){

	//We use this variables for all sensors
	//? less memory and time consumption
	b2PolygonShape tmpPolygonShape;
	b2FixtureDef tmpFixtureDef;

	//We set this one time, should work
	tmpFixtureDef.shape = &tmpPolygonShape;
	tmpFixtureDef.isSensor = true;

	//! Create Foot Sensor
	//? Width  : same as player collider (16 Pixel)
	//? Height : Not to height (8 Pixel)

	tmpPolygonShape.SetAsBox(8/PTM_RATIO, 8/PTM_RATIO, b2Vec2(0,-16/PTM_RATIO+16.0f/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)ENEMY_FOOT );

	//! EndOf Foot Sensor

	tmpPolygonShape.SetAsBox(8/PTM_RATIO, 8/PTM_RATIO, b2Vec2(-24.0f/PTM_RATIO, 16.0f/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)ENEMY_FRONT_FOOT );

	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 32/PTM_RATIO, b2Vec2(-88.0f/PTM_RATIO, 192/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)ENEMY_OVER_HEAD_1 );

	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 32/PTM_RATIO, b2Vec2(-88.0f/PTM_RATIO, 256/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)ENEMY_OVER_HEAD_2 );

	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 32/PTM_RATIO, b2Vec2(-88.0f/PTM_RATIO, 320/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)ENEMY_OVER_HEAD_3 );
}