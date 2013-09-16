#include "HelloWorldScene.h"
#include "MainMenuScene.h"

#include "CCSpriterX.h"
USING_NS_CC;

Scene* HelloWorldScene::scene()
{
    // 'scene' is an autorelease objec
    Scene *scene = Scene::create();

    // 'layer' is an autorelease object
    HelloWorldScene *layer = HelloWorldScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorldScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    MenuItemImage *closeItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(HelloWorldScene::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    Menu* menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    LabelTTF* label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

	//SpriteBatchNode *n = SpriteBatchNode::create("monster.png");
    CCSpriterX *animator = CCSpriterX::create("Example.SCML", "monster.plist");

	animator->setPosition(ccp(240, 30));
	animator->setScale(0.8f);
		
	//n->addChild(animator);
	this->addChild(animator, 5);

    
    return true;
}


void HelloWorldScene::menuCloseCallback(Object* pSender)
{
    //Director::getInstance()->end();

	Director::getInstance()->replaceScene(MainMenuScene::scene());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
