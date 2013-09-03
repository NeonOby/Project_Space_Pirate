#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class Collider
{
protected:
	void createBody(b2World* _world);

	b2Body* body;
};

#endif  // __BAUTEIL_H__