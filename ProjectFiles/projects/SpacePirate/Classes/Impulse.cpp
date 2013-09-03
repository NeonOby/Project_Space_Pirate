#include "Impulse.h"

using namespace cocos2d;

Impulse::Impulse(b2World* _world, CCPoint pos, CCPoint pos2, int str){
	speedMult = 10.0f;
	strength = str;
	emitter = CCParticleSystemQuad::create("impulse.plist");
	emitter->setStartSize(IMPULSE_SIZE*(str*0.1f+1.0f));
	emitter->setPositionType(kCCPositionTypeFree);
	emitter->setPosition(pos);
	node = emitter;

	createBody(_world);

	ApplyForceTo(pos2);
}

void Impulse::createBody(b2World* _world){
	b2BodyDef paddleBodyDef;
	paddleBodyDef.type = b2_dynamicBody;
	paddleBodyDef.position.Set(node->getPositionX()/PTM_RATIO, node->getPositionY()/PTM_RATIO);
	paddleBodyDef.userData = node;

	body = _world->CreateBody(&paddleBodyDef);

	// Create paddle shape
	b2PolygonShape paddleShape;
      paddleShape.SetAsBox((IMPULSE_SIZE*(strength*0.1f+1.0f))/PTM_RATIO/2,
                           (IMPULSE_SIZE*(strength*0.1f+1.0f))/PTM_RATIO/2);

	// Create shape definition and add to body
	b2FixtureDef paddleShapeDef;
	paddleShapeDef.shape = &paddleShape;
	paddleShapeDef.density = 15.0f;
	paddleShapeDef.friction = 0.0f;
	paddleShapeDef.restitution = 0.1f;

	paddleShapeDef.filter.categoryBits = IMPULSE;
	paddleShapeDef.filter.maskBits = SHIP | ENEMY | ASTEROID;

	b2Fixture *_paddleFixture;
	_paddleFixture = body->CreateFixture(&paddleShapeDef);
}


