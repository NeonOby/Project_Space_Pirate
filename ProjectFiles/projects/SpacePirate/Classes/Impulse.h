#include "MoveAble.h"

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class Impulse : public MoveAble
{
public:
	Impulse(b2World* _world, cocos2d::CCPoint pos, cocos2d::CCPoint pos2, int str);

	cocos2d::CCParticleSystemQuad* getEmitter(){
		if(emitter)
			return emitter;
		else
			return NULL;
	};

protected:
	void createBody(b2World* _world);

	int strength;
	cocos2d::CCParticleSystemQuad* emitter;
};