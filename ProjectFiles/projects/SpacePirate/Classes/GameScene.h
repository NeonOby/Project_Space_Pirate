#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "ParallaxLayer.h"
#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include <vector>
#include "MyContactListener.h"

class GameScene : public cocos2d::Layer {
private:
	ParallaxLayer* parallaxLayer;

	cocos2d::Layer* level1;
	cocos2d::Layer* level2;
	cocos2d::Layer* level3;

	cocos2d::Sprite* player;

	cocos2d::Sprite** blocks;

	cocos2d::Sprite* himmel;

	MyContactListener myContactListenerInstance;

	b2Body * createPlatform(float x, float y, float width, float height);
	b2Body * createBullet(float x, float y, float width, float height);
	b2Body * createKiste(float x, float y, float width, float height);
	void ShootBullet();

	
	bool jumping;
	bool climbingRight, climbingLeft, holdingRight, holdingLeft;
	float fallTime, waitTime, slowTime, jumpStart, jumpTimer;
	int walkDirection;

	cocos2d::Point PlayerSpawn;

	cocos2d::Sprite *Player;

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

