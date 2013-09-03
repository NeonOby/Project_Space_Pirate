#ifndef __DAMAGEABLE_H__
#define __DAMAGEABLE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class DamageAble
{
public:
	void damage(int amount){
		health-=amount;
		if(health<0)
			health=0;
	};

	void heal(int amount){
		health+=amount;
		if(health>maxHealth)
			health=maxHealth;
	};

	int getHealth(){
		return health;
	};

	int getMaxHealth(){
		return maxHealth;
	};

	void setMaxHealth(int value){
		maxHealth = value;
	};

	void setHealth(int value){
		health = value;
	};

	bool isDestroyed(){
		return (health<=0);
	};

	void impact(int side);
protected:
	int health;
	int maxHealth;
};

#endif  // __DAMAGEABLE_H__