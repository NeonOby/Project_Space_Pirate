#include "GameScene.h"
#include "irrKlang.h"
#include "Box2D/Box2D.h"
#include "Physic/B2DebugDrawLayer.h"
#include "Manuel/SoundManager.h"

#include "MouseListener.h"

#include "Level/LevelMap.h"

#include "conio.h"

#include "Konstanten.h"
#include "Player.h"
#include "GameGUI.h"

USING_NS_CC;
using namespace irrklang;

b2World *_world;
//The Body of the Sprite, which is the collision shape if you want so

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
	climbingLeft = false;
	climbingRight = false;
	jumping = false;
	fallTime = 0;
	waitTime = 0;
	walkDirection = 1;
	jumpStart = 0;
	slowTime = 0;
	jumpTimer = 0;
	holdingRight = false;
	holdingLeft = false;
	mPlayer = NULL;
	level1 = new Layer();
	level2 = new Layer();
	level3 = new Layer();
	blocks = new Sprite*[200];
	
	himmel = Sprite::create("Level/Jungle/Himmel.PNG");

	// position the sprite on the center of the screen
	himmel->setPosition(Point(himmel->getContentSize().width/2*0.5f, himmel->getContentSize().height/2*0.5f));

	// add the sprite as a child to this layer
	this->addChild(himmel, 0);

	parallaxLayer = ParallaxLayer::create();
	this->addChild(parallaxLayer, 1);
	this->addChild(level1, 2);
	this->addChild(level2, 3);
	this->addChild(level3, 4);

	this->addChild(GameGUI::create(),5);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	//Try to play sound here

	SoundManager::playMusic(COMPLEX);

	// Define the gravity vector.
	b2Vec2 gravity;
	gravity.Set(0.0f, -GRAVITY_Y);//No gravity
 
	// create a world object, which will hold and simulate the rigid bodies.
	_world = new b2World(gravity);
	//World is generated, now add some things
	

	LevelMap map = LevelMap(_world);
	map.SetLayerArray(level1,level2,level3);
	map.SetParallaxLayer(parallaxLayer->getLayer(0),parallaxLayer->getLayer(1),parallaxLayer->getLayer(2));
	if(map.LoadMap("resources/maps/map6.xml")){
			log("loading map finished: success");
			PlayerSpawn = map.getSpawnPoint();
	}else{
			log("loading map finished: Error");
			PlayerSpawn = Point(100,200);
	}


	//Map Finished now set SpawnPos:
	//create World and add something
	//The world, world has Gravity etc.
	
	//The sprite of the ball, it moves, rotates, scales with the shape (_ball is not controlled by physic)
	


	//!++ Create Player Here:
	
	mPlayer = Player::create(_world, &PlayerSpawn);
	level2->addChild(mPlayer->GetSprite());

	myContactListenerInstance = MyContactListener(mPlayer->GetSprite());
	_world->SetContactListener(&myContactListenerInstance);

	//Player BOdy !!

	


	//Its the DEBUG Layer for Box2D which draws Debug Thingies
	if(BOX2D_DEBUG)
		this->addChild(B2DebugDrawLayer::create(_world, PTM_RATIO),999);
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

	this->schedule(schedule_selector(GameScene::step), BOX2D_STEP);

	this->schedule(schedule_selector(GameScene::update), 0.01F);

	_world->SetContinuousPhysics(false);

	//This follows the Player !!
	//this->runAction(Follow::create(_ball));

	return true;
}

void GameScene::ShootBullet(){
	Sprite* Player = (Sprite*)mPlayer->GetBody()->GetUserData();
	b2Body * tmpBody = createBullet((Player->getPositionX()+walkDirection*48),Player->getPositionY()+32,8.0f,8.0f);

	Point tmpPoint = MouseListener().getMousePos();

	float dX = tmpPoint.x-(Player->getPositionX()+walkDirection);
	float dY = tmpPoint.y-Player->getPositionY();

	tmpBody->ApplyForceToCenter(b2Vec2(dX,dY));

}

void GameScene::grappleHook(){

}

b2Body * GameScene::createBullet(float x, float y, float width, float height){
	//Floor2 Body only (no Sprite yet)
	b2Body *_body;
	

	b2BodyDef BodyDef;
	BodyDef.type = b2_dynamicBody;
	BodyDef.bullet = true;
	
	BodyDef.position.Set(x/PTM_RATIO, y/PTM_RATIO);
	_body = _world->CreateBody(&BodyDef);

	b2PolygonShape shapeDef;
	shapeDef.SetAsBox(width/PTM_RATIO, height/PTM_RATIO);
 
	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &shapeDef;
	ballShapeDef.isSensor = true;
	b2Fixture* leftSideSensorFixture =_body->CreateFixture(&ballShapeDef);
	leftSideSensorFixture->SetUserData((void*)BULLET);

	return _body;
}

void GameScene::step(float dt){
	if (GetAsyncKeyState(VK_RBUTTON)){
		grappleHook();
	}


	if(GetAsyncKeyState(VK_LBUTTON)){
		ShootBullet();
	}

	_world->Step(dt, 10, 10);

	//Get List from Contact Listener and delete those
	vector<BulletHit> *list = &myContactListenerInstance.mBulletHits;
	vector<b2Body*> *destroy = new vector<b2Body*>();

	for (std::vector<BulletHit>::iterator it = list->begin() ; it != list->end(); ++it){
		//Make Effects, apply Forces etc.

		//For now delete Shot
		if(std::find(destroy->begin(), destroy->end(), it->bulletFixture->GetBody()) == destroy->end()){
			destroy->push_back(it->bulletFixture->GetBody());
		}
		if(std::find(destroy->begin(), destroy->end(), it->hitFixture->GetBody()) == destroy->end()){
			//destroy->push_back(it->hitFixture->GetBody());
		}
	}

	for(std::vector<b2Body*>::iterator tmpBody = destroy->begin() ; tmpBody != destroy->end(); ++tmpBody){
		_world->DestroyBody(*tmpBody);
	}

	myContactListenerInstance.mBulletHits.clear();

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
}

void GameScene::update(float dt){

	

	//Test

	//Update World, Box2D updates position, rotation, calculates things etc.
	


	//Test If GameOver things
	if(mPlayer->GetSprite()->getPositionY()<-MIN_KAMERA_POS){
		//To low dead:
		mPlayer->GetBody()->SetTransform(b2Vec2(PlayerSpawn.x/PTM_RATIO, PlayerSpawn.y/PTM_RATIO), 0.0f);
		mPlayer->GetBody()->SetLinearVelocity(b2Vec2(0,0));
		waitTime = 0.15f;
		slowTime = 0.2f;
	}

	

	bool canMove = true;
	bool canJump = false;
	bool flying = false;
	bool falling = false;
	bool JUMP_PRESSED = false;

	if(GetAsyncKeyState(VK_SPACE))
		JUMP_PRESSED = true;

	if(waitTime<=0 && !jumping && jumpTimer<=0 && myContactListenerInstance.playerFootContacts>0){
		canJump = true;
	}

	float maxSpeed = PLAYER_MAX_SPEED*mPlayer->GetBody()->GetMass();
	float speed = PLAYER_SPEED_CHANGE*mPlayer->GetBody()->GetMass();
	

	//Jump
	if(canJump && JUMP_PRESSED){
		canMove = false;

		jumpStart = PLAYER_START_JUMP_TIME;
		
		if(jumpStart<=0.0f){
			mPlayer->GetBody()->SetLinearVelocity(b2Vec2(mPlayer->GetBody()->GetLinearVelocity().x, PLAYER_JUMP_SPEED*mPlayer->GetBody()->GetMass()));
			jumping = false;
			jumpTimer = PLAYER_JUMP_COOLDOWN;
		}else{
			jumping = true;
		}
	}else if(jumping){
		//Während man springt kann man sich nicht bewegen, LoL
		canMove = false;

		jumpStart -= dt;
		if(jumpStart<0.0f){
			jumpStart=0.0f;
			mPlayer->GetBody()->ApplyLinearImpulse(b2Vec2(0.0f, PLAYER_JUMP_SPEED*mPlayer->GetBody()->GetMass()), mPlayer->GetBody()->GetWorldCenter());
			jumping = false;
			jumpTimer = PLAYER_JUMP_COOLDOWN;
		}

		//Slow him down
		mPlayer->GetBody()->ApplyLinearImpulse(b2Vec2(-PLAYER_SLOW_ON_JUMP* mPlayer->GetBody()->GetLinearVelocity().x, mPlayer->GetBody()->GetLinearVelocity().y), mPlayer->GetBody()->GetWorldCenter());
	}else{
	
		//Wenn wir gerade Fallen
		if(mPlayer->GetBody()->GetLinearVelocity().y < 0 && myContactListenerInstance.playerFootContacts<=0){
			falling = true;
			fallTime+=dt;
		}else if(mPlayer->GetBody()->GetLinearVelocity().y > 0){
			flying = true;
		}

		//Unter Player ist was
		if(myContactListenerInstance.playerFootContacts>0){
			//Hit the Ground, look if we where falling
			if(fallTime > FALL_MAX_SPEED_WITHOUT_DEBUFF){
				//Hit the ground for first time after falling
				if(fallTime > FALL_MAX_SPEED_WITHOUT_DAMAGE){
					waitTime += fallTime*FALL_STOP_TIME_MULTIPLIER;
					slowTime += waitTime;
					mPlayer->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
					//TODO Fall Damage
				}

				slowTime += fallTime*FALL_DEBUF_TIME_MULTIPLIER;
			}
			fallTime=0;
		}

		//Right site climbing
		if(climbingRight && myContactListenerInstance.playerRightSideContacts>0){
			//Continuing Climbing
			mPlayer->GetBody()->SetLinearVelocity(b2Vec2(mPlayer->GetBody()->GetLinearVelocity().x, PLAYER_CLIMBING_SPEED*mPlayer->GetBody()->GetMass()));
			mPlayer->GetBody()->ApplyLinearImpulse(b2Vec2(1.0f*mPlayer->GetBody()->GetMass(), 0.0f), mPlayer->GetBody()->GetWorldCenter());
		}else if(climbingRight){
			//End climbing, add small force
			mPlayer->GetBody()->ApplyLinearImpulse(b2Vec2(0.1f*mPlayer->GetBody()->GetMass(), 0.0f), mPlayer->GetBody()->GetWorldCenter());
			climbingRight = false;
			waitTime += 0.5f;
		}
		
		//Left site climbing		
		if(climbingLeft && myContactListenerInstance.playerLeftSideContacts>0){
			//Continuing Climbing
			mPlayer->GetBody()->SetLinearVelocity(b2Vec2(mPlayer->GetBody()->GetLinearVelocity().x, PLAYER_CLIMBING_SPEED*mPlayer->GetBody()->GetMass()));
			mPlayer->GetBody()->ApplyLinearImpulse(b2Vec2(-1.0f*mPlayer->GetBody()->GetMass(), 0.0f), mPlayer->GetBody()->GetWorldCenter());
		}else if(climbingLeft){
			//End climbing, add small force
			mPlayer->GetBody()->ApplyLinearImpulse(b2Vec2(-0.1f*mPlayer->GetBody()->GetMass(), 0.0f), mPlayer->GetBody()->GetWorldCenter());
			climbingLeft = false;
			waitTime += 0.5f;
		}
	}

	if(jumpTimer >0){
		jumpTimer-=dt;
	}

	if(waitTime >0){
		waitTime-=dt;
		if(waitTime>0)
			canMove = false;
		else{
			waitTime = 0;
		}
	}

	if(slowTime > 0){
		slowTime-=dt;
		if(slowTime>0){
			speed *= FALL_DEBUF_SPEED_MULTIPLIER;
			maxSpeed *= FALL_DEBUF_SPEED_MULTIPLIER;
		}else{
			slowTime = 0;
		}
	}

	
	if(falling){
		speed *= PLAYER_SLOW_FALLING;
		maxSpeed *= PLAYER_SLOW_FALLING;
	}else if(flying){
		speed *= PLAYER_SLOW_FLYING;
		maxSpeed *= PLAYER_SLOW_FLYING;
	}

	//Get KeyInput, will be outcoded in InputManager
	if(canMove && !climbingRight && GetAsyncKeyState(VK_RIGHT)){
		mPlayer->GetSprite()->setScaleX(1);
		walkDirection = 1;
		if(!climbingRight && !holdingRight && myContactListenerInstance.playerRightStartClimbContacts>0){
			holdingRight = true;
		}
		if(holdingLeft){
			holdingLeft = false;
		}
		if(climbingLeft){
			mPlayer->GetBody()->ApplyLinearImpulse(b2Vec2(5.0f, 0.0f), mPlayer->GetBody()->GetWorldCenter());
			climbingLeft = false;
		}
		if(mPlayer->GetBody()->GetLinearVelocity().x < maxSpeed){
			mPlayer->GetBody()->ApplyLinearImpulse(b2Vec2(speed,0.0f), mPlayer->GetBody()->GetWorldCenter());
			if(mPlayer->GetBody()->GetLinearVelocity().x > maxSpeed){
				mPlayer->GetBody()->SetLinearVelocity(b2Vec2(maxSpeed, mPlayer->GetBody()->GetLinearVelocity().y));
			}
		}
			
	}else if(canMove && !climbingLeft && GetAsyncKeyState(VK_LEFT)){
		mPlayer->GetSprite()->setScaleX(-1);
		walkDirection = -1;
		if(!climbingLeft && !holdingLeft && myContactListenerInstance.playerLeftStartClimbContacts>0){
			holdingLeft = true;
		}
		if(holdingRight){
			holdingRight = false;
		}
		if(climbingRight){
			mPlayer->GetBody()->ApplyLinearImpulse(b2Vec2(-5.0f, 0.0f), mPlayer->GetBody()->GetWorldCenter());
			climbingLeft = false;
		}
		if(mPlayer->GetBody()->GetLinearVelocity().x > -maxSpeed){
			mPlayer->GetBody()->ApplyLinearImpulse(b2Vec2(-speed,0.0f), mPlayer->GetBody()->GetWorldCenter());
			if(mPlayer->GetBody()->GetLinearVelocity().x < -maxSpeed){
				mPlayer->GetBody()->SetLinearVelocity(b2Vec2(-maxSpeed, mPlayer->GetBody()->GetLinearVelocity().y));
			}
		}
	}else if(canMove && canJump){
		mPlayer->GetBody()->ApplyLinearImpulse(b2Vec2(-mPlayer->GetBody()->GetLinearVelocity().x*PLAYER_SLOW_MULTIPLIER, 0.0f), mPlayer->GetBody()->GetWorldCenter());
	}
		
	if(flying || falling){
		mPlayer->GetBody()->ApplyLinearImpulse(b2Vec2(-mPlayer->GetBody()->GetLinearVelocity().x*PLAYER_SLOW_WHILE_FLYING, 0.0f), mPlayer->GetBody()->GetWorldCenter());
	}

	//Mal sehn ob er Clmibing will
	if(holdingRight || holdingLeft){
		mPlayer->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		mPlayer->GetBody()->SetAwake(false);

		if(JUMP_PRESSED){
			if(holdingLeft)
				climbingLeft=true;
			if(holdingRight)
				climbingRight=true;
			holdingRight = false;
			holdingLeft = false;

			mPlayer->GetBody()->SetAwake(true);
			mPlayer->GetBody()->ApplyLinearImpulse(b2Vec2(0.0f, PLAYER_CLIMBING_START_JUMP*mPlayer->GetBody()->GetMass()), mPlayer->GetBody()->GetWorldCenter());
			//_Player->SetLinearVelocity(b2Vec2(_Player->GetLinearVelocity().x, PLAYER_CLIMBING_SPEED));
		}
	}


	//Update MainLayer Position to scroll with player:

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point playerPos = level2->convertToWorldSpace(mPlayer->GetSprite()->getPosition());

	float playerDiffToCenterX = (visibleSize.width/2-playerPos.x)*0.06f;

	//Have to do Absolute Y Movement
	float playerDiffToCenterY = (visibleSize.height/2-playerPos.y)*0.06f;

	//playerDiffToCenterX = 0;
	//playerDiffToCenterY = 0;
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

	if(playerDiffToCenterY>0 && this->getPositionY()>=MIN_KAMERA_POS){
		//To low
		this->setPositionY(MIN_KAMERA_POS);
	}else if(playerDiffToCenterY<0 && this->getPositionY()<=-MAX_KAMERA_POS*(MAX_MAP_HEIGHT/visibleSize.height)){
		//To high
		this->setPositionY(-MAX_KAMERA_POS*(MAX_MAP_HEIGHT/visibleSize.height));
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
