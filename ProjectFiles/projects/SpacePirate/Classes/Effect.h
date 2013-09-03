#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class Effect
{
public:
	void update(float dt);

	cocos2d::CCObject* getEffect(){
		return effect;
	};

protected:
	bool isFinished;
	cocos2d::CCObject* effect;
};

#endif  // __EFFECT_H__