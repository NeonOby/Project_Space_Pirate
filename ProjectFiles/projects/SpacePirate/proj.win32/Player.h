#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Konstanten.h"

class Player : public cocos2d::Layer {
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

	//The time the player has fallen
	//? Only gets higher if player speed in Y is <0 (falling down)
	//? Is set to 0 if player is not falling
	float mFallTimer;

	//The remaining time the player is slowed
	//? If(mSlowTimer>0) slow player
	float mSlowTimer;

	//The remaining time the player can't do something
	//? If(mWaitTimer>0) stop player from doing something
	float mWaitTimer;

	//The direction the player is facing
	//? -1 = Left, 1 = Right (You can calculate things with it)
	int mDirection;

	//If the player's feet are touching something
	//? Used to control jumping
	//? You can't jump in Air
	bool mGrounded;

	//Climbing Variables
	//? Those return if player does currently hold on an edge or climbs up
	bool mClimbingRight, \
		 mClimbingLeft, \
		 mHoldingRight, \
		 mHoldingLeft;

	//The Player b2Body (Pointer only)
	//? You can get e.g. speed from it
	b2Body *mPlayerBody;

	//The Player CCSprite
	//? Just one Sprite, because bone animation takes to long, gets animated
	cocos2d::Sprite *mPlayerSprite;

	//The current b2World
	//? Gets set in Constructor
	b2World * mWorld;

	//!+ Private Functions

	//Get all AnimationFrames (Sprites) and cache them for later use
	//? See animation-variables for animation-names
	void CacheAnimations();

	//Create the Player Sprite etc.
	//? The Sprite
	void CreateSprite();

	//Creates the physically b2Body of the Player, his collider
	//? 1 Circle at the bottom + 1 Square above
	void CreateBody(cocos2d::Point *pSpawn);

	//Creates the sensors:
	//? Feet: Jumping, Arms: Climbing
	void CreateSensors();
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, 
	//instead of returning 'id' in cocos2d-iphone
	virtual bool init(cocos2d::Point *pSpawn);

	Player(b2World *pWorld);
	~Player();

	//Implement it manually because we have a special constructor
	static Player* create(b2World *pWorld, cocos2d::Point *pSpawn);

	//? current fall time
	float GetFallTime(){ return mFallTimer; };

	bool IsGrounded(){ return mGrounded; };

	void update(float dt);
};

#endif