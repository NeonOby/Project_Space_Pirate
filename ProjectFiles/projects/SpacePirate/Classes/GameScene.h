#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "ParallaxLayer.h"
#include "cocos2d.h"

class GameScene : public cocos2d::Layer {
private:
	ParallaxLayer* parallaxLayer;

	cocos2d::Layer* level;

	cocos2d::Sprite* player;

	cocos2d::Sprite** blocks;

public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);

	void update(float dt);
};

const float speed = 300.0f;

#endif

