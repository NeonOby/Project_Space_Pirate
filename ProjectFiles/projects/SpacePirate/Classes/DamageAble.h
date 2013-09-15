#ifndef __DAMAGEABLE_H__
#define __DAMAGEABLE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class DamageAble
{
public:
	void damage(int amount){
		if(amount>0){
			cocos2d::log("DamageAble: You can't damage postive amounts: %f \n \
						 Use DamageAble::heal(int amount); instead.", amount);
			return;
		}
		
		if(health-amount<0)
			health=0;
		else
			health-=amount;
	};

	void heal(int amount){
		if(amount<0){
			cocos2d::log("DamageAble: You can't heal negative amounts: %f \n \
				Use DamageAble::damage(int amount); instead.", amount);
			return;
		}
		//If we are not over maxHealth, heal
		//If we are set it to maxHealth
		if(health+amount>maxHealth)
			health=maxHealth;
		else
			health+=amount;
	};

	int getHealth(){
		return health;
	};

	int getMaxHealth(){
		return maxHealth;
	};

	void setMaxHealth(int value){
		//We will only heal the Player if he gets more Health
		//We will heal him by the extra health he got (The difference to the last maxHealth)
		//We will not damage him, if his maxHealth gets lower
		float diff = value-maxHealth;
		maxHealth = value;
		heal(diff);
	};

	void setHealth(int value){
		if(health-value<=0)
			health = 0;
		else
			health = value;
	};

	bool isDestroyed(){
		return (health<=0);
	};
protected:
	int health;
	int maxHealth;
};

#endif  // __DAMAGEABLE_H__