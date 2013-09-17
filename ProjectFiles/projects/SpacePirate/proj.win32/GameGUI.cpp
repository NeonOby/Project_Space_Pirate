#include "GameGUI.h"

USING_NS_CC;


bool GameGUI::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	//GUI 
	LabelTTF* healthBar = LabelTTF::create("Leben", "A Damn Mess", 24);
   	healthBar->setAnchorPoint(Point(0,1));
    healthBar->setPosition(Point(0,visibleSize.height));
    this->addChild(healthBar, 5);

	LabelTTF* ammoBar = LabelTTF::create("Munition", "A Damn Mess", 24);
   	ammoBar->setAnchorPoint(Point(0,1));
    ammoBar->setPosition(Point(0,visibleSize.height - 30));
    this->addChild(ammoBar, 5);


	 

return true;
}