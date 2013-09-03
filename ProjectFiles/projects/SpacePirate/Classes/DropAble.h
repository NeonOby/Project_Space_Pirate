#ifndef __DROPABLE_H__
#define __DROPABLE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class DropAble : public cocos2d::CCSprite
{
public:
	DropAble(int type, int money){
		id = type;
		cost = money;
	};

protected:
	int id;
	int cost;
};

#endif  // __DROPABLE_H__