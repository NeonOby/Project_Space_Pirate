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
	footContact(NULL)
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
	footContact(NULL),\
	\
	leftFrontFootContact(NULL),\
	\
	leftLowBarrierListener(NULL),\
	leftHighBarrierListener(NULL),\
	\
	leftLowOverHeadListener(NULL),\
	leftMidOverHeadListener(NULL),\
	leftHighOverHeadListener(NULL),\
	\
	rightFrontFootContact(NULL),\
	\
	rightLowBarrierListener(NULL),\
	rightHighBarrierListener(NULL),\
	\
	rightLowOverHeadListener(NULL),\
	rightMidOverHeadListener(NULL),\
	rightHighOverHeadListener(NULL),\
	\
	midLowOverHeadListener(NULL),\
	midMidOverHeadListener(NULL),\
	midHighOverHeadListener(NULL)\
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




	//Only this fixtures will be affecting contact count
	int FixtureMask = CLIMBFIXTURE | KISTE | DYNAMIC_KISTE;
	footContact = MyContactListener::GetInstance()->AddListener(mEnemyBody, ENEMY_FOOT, FixtureMask);

	leftFrontFootContact = MyContactListener::GetInstance()->AddListener(mEnemyBody, LEFT_ENEMY_FRONT_FOOT, FixtureMask);

	leftLowBarrierListener = MyContactListener::GetInstance()->AddListener(mEnemyBody, LEFT_ENEMY_BARRIER_1, FixtureMask);
	leftHighBarrierListener = MyContactListener::GetInstance()->AddListener(mEnemyBody, LEFT_ENEMY_BARRIER_2, FixtureMask);

	leftLowOverHeadListener = MyContactListener::GetInstance()->AddListener(mEnemyBody, LEFT_ENEMY_OVER_HEAD_1, FixtureMask);
	leftMidOverHeadListener = MyContactListener::GetInstance()->AddListener(mEnemyBody, LEFT_ENEMY_OVER_HEAD_2, FixtureMask);
	leftHighOverHeadListener = MyContactListener::GetInstance()->AddListener(mEnemyBody, LEFT_ENEMY_OVER_HEAD_3, FixtureMask);

	rightFrontFootContact = MyContactListener::GetInstance()->AddListener(mEnemyBody, RIGHT_ENEMY_FRONT_FOOT, FixtureMask);

	rightLowBarrierListener = MyContactListener::GetInstance()->AddListener(mEnemyBody, RIGHT_ENEMY_BARRIER_1, FixtureMask);
	rightHighBarrierListener = MyContactListener::GetInstance()->AddListener(mEnemyBody, RIGHT_ENEMY_BARRIER_2, FixtureMask);

	rightLowOverHeadListener = MyContactListener::GetInstance()->AddListener(mEnemyBody, RIGHT_ENEMY_OVER_HEAD_1, FixtureMask);
	rightMidOverHeadListener = MyContactListener::GetInstance()->AddListener(mEnemyBody, RIGHT_ENEMY_OVER_HEAD_2, FixtureMask);
	rightHighOverHeadListener = MyContactListener::GetInstance()->AddListener(mEnemyBody, RIGHT_ENEMY_OVER_HEAD_3, FixtureMask);

	midLowOverHeadListener = MyContactListener::GetInstance()->AddListener(mEnemyBody, MID_ENEMY_OVER_HEAD_1, FixtureMask);
	midMidOverHeadListener = MyContactListener::GetInstance()->AddListener(mEnemyBody, MID_ENEMY_OVER_HEAD_2, FixtureMask);
	midHighOverHeadListener = MyContactListener::GetInstance()->AddListener(mEnemyBody,MID_ENEMY_OVER_HEAD_3, FixtureMask);
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
	vec.operator -= (mEnemyBody->GetPosition());

	float length = vec.Length();
	float lengthInPixls = length * PTM_RATIO;

	vec.x = vec.x / length;
	vec.y = 0;

	vec.operator*=(5);

	float Masse = mEnemyBody->GetMass();
	//log("My Mass: %f", Masse);

	vec.y = mEnemyBody->GetLinearVelocity().y;

	//TODO: enemyWalk
	if (lengthInPixls < Director::getInstance()->getVisibleSize().width/2 && *footContact > 0)
	{
		mEnemyBody->SetLinearVelocity(vec);
	}

	//TODO: enemyJUMP

	//Sprung über eine kleine Bodenkante links
	if(*leftFrontFootContact > 0 && mEnemyBody->GetLinearVelocity().y >= 0 && *footContact > 0)
	{
		mEnemyBody->SetLinearVelocity(b2Vec2(mEnemyBody->GetLinearVelocity().x, 0.15f*mEnemyBody->GetMass()));
	} else if(*leftFrontFootContact > 0)
	{
		//add small force nach links
		mEnemyBody->ApplyLinearImpulse(b2Vec2(-0.1f*mEnemyBody->GetMass(), 0.0f), mEnemyBody->GetWorldCenter());
	}

	//Sprung über eine kleine Bodenkante rechts
	if(*rightFrontFootContact > 0 && mEnemyBody->GetLinearVelocity().y >= 0 && *footContact > 0)
	{
		mEnemyBody->SetLinearVelocity(b2Vec2(mEnemyBody->GetLinearVelocity().x, 0.15f*mEnemyBody->GetMass()));
	} else if(*leftFrontFootContact > 0)
	{
		//add small force nach links
		mEnemyBody->ApplyLinearImpulse(b2Vec2(-0.1f*mEnemyBody->GetMass(), 0.0f), mEnemyBody->GetWorldCenter());
	}

	//Sprung über kleine Hindernisse in 2 Höhen nach links
	if(*leftLowBarrierListener > 0 && mEnemyBody->GetLinearVelocity().y >= 0 && *footContact > 0)
	{
		mEnemyBody->ApplyForceToCenter(b2Vec2(mEnemyBody->GetLinearVelocity().x, 250*mEnemyBody->GetMass()));
	}
	
	if(*leftHighBarrierListener > 0 && mEnemyBody->GetLinearVelocity().y >= 0 && *footContact > 0)
	{
		mEnemyBody->ApplyForceToCenter(b2Vec2(mEnemyBody->GetLinearVelocity().x, 300*mEnemyBody->GetMass()));
	}

	//Sprung über kleine Hindernisse in 2 Höhen nach rechts

	if(*rightLowBarrierListener > 0 && mEnemyBody->GetLinearVelocity().y >= 0 && *footContact > 0)
	{
		mEnemyBody->ApplyForceToCenter(b2Vec2(mEnemyBody->GetLinearVelocity().x, 250*mEnemyBody->GetMass()));
	}

	if(*rightHighBarrierListener > 0 && mEnemyBody->GetLinearVelocity().y >= 0 && *footContact > 0)
	{
		mEnemyBody->ApplyForceToCenter(b2Vec2(mEnemyBody->GetLinearVelocity().x, 300*mEnemyBody->GetMass()));
	}
		log("niedrig: %i", *midLowOverHeadListener);

	//Sprung auf Plattformen in 3 Höhen nach links
	if(*leftLowOverHeadListener > 0 && mEnemyBody->GetLinearVelocity().y >= 0 && mEnemyBody->GetLinearVelocity().x < 0 && *footContact > 0)
	{
		if(*midLowOverHeadListener > 0){
		} else{
			mEnemyBody->ApplyForceToCenter(b2Vec2(0, 320.0f*mEnemyBody->GetMass()));
		}
	}
		log("mittel: %i", *midMidOverHeadListener);

	if(*leftMidOverHeadListener > 0 && mEnemyBody->GetLinearVelocity().y >= 0 && mEnemyBody->GetLinearVelocity().x < 0 && *footContact > 0)
	{
		if(*midMidOverHeadListener > 0)
		{
		} else
		{
			mEnemyBody->ApplyForceToCenter(b2Vec2(0, 460.0*mEnemyBody->GetMass()));
		}
	}
		log("hoch: %i", *midHighOverHeadListener);

	if(*leftHighOverHeadListener > 0 && mEnemyBody->GetLinearVelocity().y >= 0 && mEnemyBody->GetLinearVelocity().x < 0 && *footContact > 0)
	{
		if(*midHighOverHeadListener > 0)
		{
		} else
		{
			mEnemyBody->ApplyForceToCenter(b2Vec2(0, 500.0f*mEnemyBody->GetMass()));
		}
	}
		log("niedrig: %i", *midLowOverHeadListener);

	//Sprung auf Plattformen in 3 Höhen nach rechts
	if(*rightLowOverHeadListener > 0 && mEnemyBody->GetLinearVelocity().y >= 0 && mEnemyBody->GetLinearVelocity().x > 0 && *footContact > 0)
	{
		if(*midLowOverHeadListener > 0)
		{
		} else
		{	
			mEnemyBody->ApplyForceToCenter(b2Vec2(0, 320.0f*mEnemyBody->GetMass()));
		}
	}
		log("mittel: %i", *midMidOverHeadListener);

	if(*rightMidOverHeadListener > 0 && mEnemyBody->GetLinearVelocity().y >= 0 && mEnemyBody->GetLinearVelocity().x > 0 && *footContact > 0)
	{
		if(*midMidOverHeadListener > 0)
		{
		} else
		{	
			mEnemyBody->ApplyForceToCenter(b2Vec2(0, 460.0f*mEnemyBody->GetMass()));
		}
	}
		log("hoch: %i", *midHighOverHeadListener);

	if(*rightHighOverHeadListener > 0 && mEnemyBody->GetLinearVelocity().y >= 0 && mEnemyBody->GetLinearVelocity().x > 0 && *footContact > 0)
	{
		if(*midHighOverHeadListener > 0)
		{
		} else
		{
			mEnemyBody->ApplyForceToCenter(b2Vec2(0, 500.0f*mEnemyBody->GetMass()));
			
		}
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

	tmpPolygonShape.SetAsBox(8/PTM_RATIO, 8/PTM_RATIO, b2Vec2(0,-16/PTM_RATIO+16/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)ENEMY_FOOT );

	//! EndOf Foot Sensor

	//left sensors

	//left foot-sensor
	tmpPolygonShape.SetAsBox(8/PTM_RATIO, 12/PTM_RATIO, b2Vec2(-24/PTM_RATIO, 20/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)LEFT_ENEMY_FRONT_FOOT );

	//left barrier-sensors
	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 24/PTM_RATIO, b2Vec2(-40/PTM_RATIO, 60/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)LEFT_ENEMY_BARRIER_1 );

	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 24/PTM_RATIO, b2Vec2(-56/PTM_RATIO, 110/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)LEFT_ENEMY_BARRIER_2 );

	//left overhead-sensors
	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 32/PTM_RATIO, b2Vec2(-96/PTM_RATIO, 192/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)LEFT_ENEMY_OVER_HEAD_1 );

	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 32/PTM_RATIO, b2Vec2(-112/PTM_RATIO, 256/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)LEFT_ENEMY_OVER_HEAD_2 );

	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 32/PTM_RATIO, b2Vec2(-128.0f/PTM_RATIO, 320/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)LEFT_ENEMY_OVER_HEAD_3 );

	//right sensors

	//right foot-sensor
	tmpPolygonShape.SetAsBox(8/PTM_RATIO, 12/PTM_RATIO, b2Vec2(24/PTM_RATIO, 20/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)RIGHT_ENEMY_FRONT_FOOT );

	//right barrier-sensors
	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 24/PTM_RATIO, b2Vec2(40/PTM_RATIO, 60/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)RIGHT_ENEMY_BARRIER_1 );

	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 24/PTM_RATIO, b2Vec2(56/PTM_RATIO, 110/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)RIGHT_ENEMY_BARRIER_2 );

	//right overhead-sensors
	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 32/PTM_RATIO, b2Vec2(96/PTM_RATIO, 192/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)RIGHT_ENEMY_OVER_HEAD_1 );

	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 32/PTM_RATIO, b2Vec2(112/PTM_RATIO, 256/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)RIGHT_ENEMY_OVER_HEAD_2 );

	tmpPolygonShape.SetAsBox(4/PTM_RATIO, 32/PTM_RATIO, b2Vec2(128/PTM_RATIO, 320/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)RIGHT_ENEMY_OVER_HEAD_3 );

	//mid overhead-sensors
	tmpPolygonShape.SetAsBox(68/PTM_RATIO, 32/PTM_RATIO, b2Vec2(0/PTM_RATIO, 192/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)RIGHT_ENEMY_OVER_HEAD_1 );

	tmpPolygonShape.SetAsBox(84/PTM_RATIO, 32/PTM_RATIO, b2Vec2(0/PTM_RATIO, 256/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)RIGHT_ENEMY_OVER_HEAD_2 );

	tmpPolygonShape.SetAsBox(100/PTM_RATIO, 32/PTM_RATIO, b2Vec2(0/PTM_RATIO, 320/PTM_RATIO), 0);
	mEnemyBody->CreateFixture(&tmpFixtureDef)->SetUserData( (void*)RIGHT_ENEMY_OVER_HEAD_3 );
}