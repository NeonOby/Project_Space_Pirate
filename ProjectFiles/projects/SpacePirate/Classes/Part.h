#ifndef __PART_H__
#define __PART_H__

#include "DestroyAble.h"
#include "Konstanten.h"
#include "Collider.h"

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class Part : public DestroyAble, public Konstanten, public Collider
{
public:
	//Part(int x, int y);
	Part(int x, int y);
	Part();

	void update(float dt);

	void draw();
	
	void initPart();

	void deInitPart();

	bool isInitiated(){
		return initiated;
	};

	cocos2d::CCRect* getRect();

	void createBody(b2World* _world);
	cocos2d::CCNode* sprite;

	cocos2d::CCPoint getPosition(){
		return position;
	};

	void setIndex(int x, int y){
		dx = x;
		dy = y;
	};

	int dx;
	int dy;

	bool isMouseOver(){
		return mouseOver;
	};

	void isMouseOver(bool b){
		mouseOver = b;
	};

protected:
	bool initiated;
	cocos2d::CCPoint position;
	bool mouseOver;
};

#endif  // __PART_H__