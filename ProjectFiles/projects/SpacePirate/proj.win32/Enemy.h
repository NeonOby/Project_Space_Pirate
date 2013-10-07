#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Konstanten.h"
#include "MyContactListener.h"

class Enemy : public cocos2d::Layer {
private:
	//!+ Private Variables

	//Part of wiki:Player---Steuerung
	//https://github.com/ADoby/Project_Space_Pirate/wiki/Player---Steuerung
	//Extension/Plugin: SourceLinks

	//The time remaining for the jumpstart
	//? Getting into position, slow down, etc.
	float mJumpStartTimer;

	//Jump cooldown remaining in seconds
	//? Normaly you can't jump directly after you just jumped
	float mJumpCooldownTimer;

	//The time the enemy has fallen
	//? Only gets higher if enemy speed in Y is <0 (falling down)
	//? Is set to 0 if enemy is not falling
	float mFallTimer;

	//The remaining time the enemy is slowed
	//? If(mSlowTimer>0) slow enemy
	float mSlowTimer;

	//The remaining time the enemy can't do something
	//? If(mWaitTimer>0) stop enemy from doing something
	float mWaitTimer;

	//The direction the enemy is facing
	//? -1 = Left, 1 = Right (You can calculate things with it)
	int mDirection;

	//If the enemy's feet are touching something
	//? Used to control jumping
	//? You can't jump in Air
	bool mGrounded;

	//Climbing Variables
	//	//? Those return if enemy does currently hold on an edge or climbs up
	//	bool mClimbingRight, \
	//		 mClimbingLeft, \
	//		 mHoldingRight, \
	//		 mHoldingLeft;

	//The current b2World
	//? Gets set in Constructor
	b2World * mWorld;

	//The Enemy b2Body (Pointer only)
	//? You can get e.g. speed from it
	b2Body*mEnemyBody;

	//The Enemy CCSprite
	//? Just one Sprite, because bone animation takes to long, gets animated
	cocos2d::Sprite *mEnemySprite;

	b2Body*mPlayer;

	//!+ Private Functions

	//Get all AnimationFrames (Sprites) and cache them for later use
	//? See animation-variables for animation-names
	void CacheAnimations();

	//Create the Enemy Sprite etc.
	//? The Sprite
	void CreateSprite();

	//Creates the physically b2Body of the enemy, his collider
	//? 1 Circle at the bottom + 1 Square above
	void CreateBody(cocos2d::Point *pSpawn);

	//Creates the sensors:
	//feet: walking/jumping, eyes->Listeners: seeing where to jump
	void CreateSensors();

	int* footContact;

	int* leftFrontFootContact;

	int* leftLowBarrierListener;
	int* leftHighBarrierListener;

	int* leftLowOverHeadListener;
	int* leftMidOverHeadListener;
	int* leftHighOverHeadListener;

	int* rightFrontFootContact;

	int* rightLowBarrierListener;
	int* rightHighBarrierListener;

	int* rightLowOverHeadListener;
	int* rightMidOverHeadListener;
	int* rightHighOverHeadListener;

public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, 
	//instead of returning 'id' in cocos2d-iphone
	virtual bool init(cocos2d::Point *pSpawn);

	Enemy();
	Enemy(b2World *pWorld, cocos2d::Point *pSpawn, b2Body* pPlayer);
	~Enemy();

	//Implement it manually because we have a special constructor
	static Enemy* create(b2World *pWorld, cocos2d::Point *pSpawn, b2Body* pPlayer);

	//? current fall time
	float GetFallTime(){ return mFallTimer; };

	bool IsGrounded(){ return mGrounded; };

	b2Body *GetBody(){ return mEnemyBody; };

	cocos2d::Sprite *GetSprite(){ return mEnemySprite; };

	void update(float dt);
};
#endif