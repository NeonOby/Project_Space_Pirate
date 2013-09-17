#ifndef __GAME_GUI_H__
#define __Game_GUI_H__


#include "cocos2d.h"



class GameGUI : public cocos2d::Layer {
private:

	

public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(GameGUI);

};


#endif
