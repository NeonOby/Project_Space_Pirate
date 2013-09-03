#ifndef __MOVEABLE_H__
#define __MOVEABLE_H__
#include "Konstanten.h"
#include "Collider.h"
#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class MoveAble : public Konstanten, public Collider
{
public:
	void ApplyForce(b2Vec2 vector){
		body->ApplyForceToCenter(vector);
	};

	void moveForward(float speed){
		b2Vec2 force = b2Vec2(cos(body->GetAngle()) * speed , sin(body->GetAngle()) * speed);
		ApplyForce(force);
	};

	void moveForward(){
		float magnitude=2.5f*speedMult;
		b2Vec2 force = b2Vec2(cos(body->GetAngle()) * magnitude , sin(body->GetAngle()) * magnitude);
		ApplyForce(force);
	};

	void ApplyForceTo(cocos2d::CCPoint pos){
		float speed=6000.0f*speedMult;

		float dx = pos.x-node->getPositionX();
		float dy = pos.y-node->getPositionY();
	
		cocos2d::CCPoint normal = cocos2d::ccpNormalize(cocos2d::Point(dx,dy));
		b2Vec2 force = b2Vec2(speed*normal.x,speed*normal.y);
		ApplyForce(force);
	};

	void update(float dt){
		node->setPosition(cocos2d::Point( body->GetPosition().x*PTM_RATIO , body->GetPosition().y*PTM_RATIO));
		node->setRotation(-CC_RADIANS_TO_DEGREES(body->GetAngle()));
		extendedUpdate(dt);
	};

	cocos2d::CCNode* getNode(){
		return node;
	};



protected:
	void createBody(b2World* _world);

	void extendedUpdate(float dt);

	cocos2d::CCNode* node;

	float speedMult;
};

#endif //__MOVEABLE_H__