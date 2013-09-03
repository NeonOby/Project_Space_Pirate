#ifndef __DESTROYABLE_H__
#define __DESTROYABLE_H__

#include "DropAble.h"
#include "Effect.h"
#include "DamageAble.h"

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class DestroyAble : public DamageAble
{
public:

	void setDrops(cocos2d::CCArray* args){
		drops = new cocos2d::CCArray;
		drops->addObjectsFromArray(args);
	};

	cocos2d::CCArray* getDrops(){
		return drops;
	};

	void addDrop(DropAble* drop){
		drops->addObject(drop);
	};

	void setEffectDestroy(Effect effect){
		effect_Destroy = effect;
	};

	Effect getEffectOnDestroy(){
		return effect_Destroy;
	};

	void impact(int side);
protected:
	cocos2d::CCArray* drops;
	Effect effect_Destroy;
};

#endif  // __DESTROYABLE_H__