#ifndef __PARALLAX_LAYER_H__
#define __PARALLAX_LAYER_H__

#include "cocos2d.h"

class ParallaxLayer : public cocos2d::Layer{
private:
	cocos2d::Layer* backgroundLayer;
	cocos2d::Layer* midground1Layer;
	cocos2d::Layer* midground2Layer;

	cocos2d::Sprite** BackGroundSprites;
	cocos2d::Sprite** MidGround1Sprites;
	cocos2d::Sprite** MidGround2Sprites;

	cocos2d::Sprite* VisibleBackgroundSprites[2];
	cocos2d::Sprite* VisibleMidground1Sprites[2];
	cocos2d::Sprite* VisibleMidground2Sprites[2];

	int nextBackground;
	int nextMidground1;
	int nextMidground2;

	void InitSprites(cocos2d::Layer* layer, const char** spriteNames, const int spriteCount, cocos2d::Sprite** spriteArray, cocos2d::Sprite** visibleSpriteArray, const int* sequence);

public:
	 virtual bool init();  

	void move(float dt, int direction);

	CREATE_FUNC(ParallaxLayer);
};

#endif