#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class Player : public cocos2d::Layer {
private:
	bool jumping;
	bool climbingRight, climbingLeft, holdingRight, holdingLeft;
	int walkDirection;

	//issue:17 //Das isn Link zu nem GitHub Issue/Bug, nicht schlecht

	//Part of wiki:Player---Steuerung
	//https://github.com/ADoby/Project_Space_Pirate/wiki/Player---Steuerung

	//The time remaining for the jumpstart
	//? Getting into position, slow down, etc.
	float mJumpStartTimer;

	//Jump cooldown remaining in seconds
	//? Normaly you can't jump directly after you just jumped
	float mJumpCooldownTimer;

	//The time the player has fallen
	//? Only gets higher if player speed in Y is <0 (falling down)
	//? Gets 0 if player is not falling
	float mFallTime;

	//The remaining time the player is slowed
	//? If(mSlowTime>0) slow player
	float mSlowTime;

	//The remaining time the player can't do something
	//? If(mWaitTime>0) stop player from doing something
	float mWaitTime;

	//The direction the player is facing
	//? -1 = Left, 1 = Right (You can calculate things with it)
	int mDirection;


	void createBody();
	void createSensors();

public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static node()" method manually
	CREATE_FUNC(Player);

	//? mFallTime
	float GetFallTime(){ return mFallTime; };

	void update(float dt);
};

float mJumpStartTimer = 0;

#endif