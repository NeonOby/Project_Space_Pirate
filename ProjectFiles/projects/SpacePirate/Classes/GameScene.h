#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "ParallaxLayer.h"
#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include <vector>
#include "MyContactListener.h"
#include "Enemy.h"

class GameScene : public cocos2d::Layer {
private:
	ParallaxLayer* parallaxLayer;

	cocos2d::Layer* level1;
	cocos2d::Layer* level2;
	cocos2d::Layer* level3;

	cocos2d::Sprite* Player;

	cocos2d::Sprite** blocks;

	cocos2d::Sprite* himmel;

	MyContactListener *myContactListenerInstance;

	b2Body * createPlatform(float x, float y, float width, float height);
	b2Body * createBullet(float x, float y, float width, float height, cocos2d::Sprite *sprite);
	b2Body * createKiste(float x, float y, float width, float height);
	
	b2Body * ShootAnker();
	b2Body * createAnker(float x, float y, float width, float height, cocos2d::Sprite *sprite);
	b2Body * Anker;

	void ShootBullet();
	
	bool jumping;
	bool climbingRight, climbingLeft, holdingRight, holdingLeft;
	float fallTime, waitTime, slowTime, jumpStart, jumpTimer;
	int walkDirection;
	float shootCooldown;

	bool hooking;
	bool hookLanding;

	int *playerFootContacts;
	int *playerRightSideContacts;
	int *playerRightStartClimbContacts;

	int *playerLeftSideContacts;
	int *playerLeftStartClimbContacts;

	b2Body *HookingObject;

	cocos2d::Point PlayerSpawn;

	b2Body *_Player;

	cocos2d::Point GetMousePos();

	Enemy** enemies;

public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* scene();
	
	// a selector callback
	void menuCloseCallback(Object* pSender);
	
	// implement the "static node()" method manually
	CREATE_FUNC(GameScene);

	void update(float dt);
	void step(float dt);
};

const float speed = 300.0f;

#endif

