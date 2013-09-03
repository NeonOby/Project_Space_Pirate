#include "Part.h"

using namespace cocos2d;

	Part::Part(int x, int y){
		position = ccp(x,y);
		initiated = false;
		mouseOver = false;
	}

	void Part::draw(){
		if(!initiated){
			//Draw Building rectangle
			//glEnable(GL_LINE_SMOOTH);
			int x = position.x;
			int y = position.y;
			glLineWidth(2.0f);
			if(mouseOver){
				cocos2d::ccDrawColor4F(0.5f, 1.0f, 0.5f, 1.0f);
			}else{
				cocos2d::ccDrawColor4F(0.5f, 0.5f, 0.5f, 1.0f);
			}
			
			ccDrawRect(ccp(x-31,y-31),ccp(x+31,y+31));
		}
	}

	CCRect* Part::getRect(){
		return new CCRect(position.x,position.y,64,64);
	}

	void Part::update(float dt){

	}

	void Part::initPart(){
		sprite = CCSprite::create("ShipPartSmall.jpg");
		sprite->setPosition(position);
		initiated = true;
	}

	void Part::deInitPart(){
		sprite = CCNode::create();
		sprite->setPosition(position);
		initiated = false;
	}

	void Part::createBody(b2World* _world){
		
	}