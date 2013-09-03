#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();


	//Hey was geht denn ? :D

	Sprite * tmpSprite;

	//Para Layer 2 (Ganz Hinten)


	Sprite* sprite = Sprite::create("Level/Jungle/Himmel.PNG");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(sprite->getContentSize().width/2*0.5f, sprite->getContentSize().height/2*0.5f));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

	parallaxLayer = ParallaxLayer::create();

	this->addChild(parallaxLayer, 1);

	level = new Layer();

	blocks = new Sprite*[200];

	for(int i=0; i< 200;i++){
		blocks[i] = Sprite::create("Game/Dirt.PNG");

		blocks[i]->setPosition(Point(blocks[i]->getContentSize().width/2 + i * blocks[i]->getContentSize().width - 100*blocks[i]->getContentSize().width,
									 blocks[i]->getContentSize().height/2));

		level->addChild(blocks[i]);
	}

	this->addChild(level, 2);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    MenuItemImage *closeItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(GameScene::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    Menu* menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    
    LabelTTF* label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

	/*
    // add "HelloWorld" splash screen"
    Sprite* sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    */

	this->schedule(schedule_selector(GameScene::update), 0.005F);

    return true;
}

void GameScene::update(float dt){
	if(GetAsyncKeyState(VK_RIGHT)){
		parallaxLayer->move(dt, -1);
		level->setPositionX(level->getPositionX()-dt*speed);
	}else if(GetAsyncKeyState(VK_LEFT)){
		parallaxLayer->move(dt, 1);
		level->setPositionX(level->getPositionX()+dt*speed);
	}

	/*
	if(GetAsyncKeyState(VK_RIGHT)){
		paralax1->setPositionX(paralax1->getPositionX()-dt*0.75F*speed);
		paralax2->setPositionX(paralax2->getPositionX()-dt*0.5F*speed);

		level->setPositionX(level->getPositionX()-dt*speed);


		//Wir suchen die Position des ersten ParalaxSprites in Relation zum Ursprung (Unten Links vom Screen)
		//Deshalb MÜSSEN wir "convertToWorldSpace" verwenden, da das Bild nicht direkt in der Scene (dem MainLayer) liegt
		//sondern in einem extra Paralax-Layer
		if(paralax2->convertToWorldSpace(paralax21->getPosition()).x + paralax21->getContentSize().width/2 <= 0){
			//paralax21 Links außerhalb vom sichtbaren Bereich
			//Verschiebe paralax21 Sprite rechts hinter das paralax22
			paralax21->setPositionX(paralax22->getPositionX() + paralax21->getContentSize().width);
		}
		if(paralax2->convertToWorldSpace(paralax22->getPosition()).x + paralax22->getContentSize().width/2 <= 0){
			//paralax22 Links außerhalb vom sichtbaren Bereich
			//Verschiebe paralax22 Sprite rechts hinter das paralax21
			paralax22->setPositionX(paralax21->getPositionX() + paralax22->getContentSize().width);
		}

		if(paralax1->convertToWorldSpace(paralax11->getPosition()).x + paralax11->getContentSize().width/2 <= 0){
			//paralax11 rechts außerhalb vom sichtbaren Bereich
			//Verschiebe paralax11 Sprite rechts hinter das paralax12
			paralax11->setPositionX(paralax12->getPositionX() + paralax11->getContentSize().width);
		}
		if(paralax1->convertToWorldSpace(paralax12->getPosition()).x + paralax12->getContentSize().width/2 <= 0){
			//paralax12 rechts außerhalb vom sichtbaren Bereich
			//Verschiebe paralax12 Sprite rechts hinter das paralax11
			paralax12->setPositionX(paralax11->getPositionX() + paralax12->getContentSize().width);
		}
	}else if(GetAsyncKeyState(VK_LEFT)){
		paralax1->setPositionX(paralax1->getPositionX()+dt*0.75F*speed);
		paralax2->setPositionX(paralax2->getPositionX()+dt*0.5F*speed);

		level->setPositionX(level->getPositionX()+dt*speed);

		//Wir suchen die Position des ersten ParalaxSprites in Relation zum Ursprung (Unten Links vom Screen)
		//Deshalb MÜSSEN wir "convertToWorldSpace" verwenden, da das Bild nicht direkt in der Scene (dem MainLayer) liegt
		//sondern in einem extra Paralax-Layer
		if(paralax2->convertToWorldSpace(paralax21->getPosition()).x - paralax21->getContentSize().width/2 >= Director::getInstance()->getVisibleSize().width){
			//paralax21 rinks außerhalb vom sichtbaren Bereich
			//Verschiebe paralax21 Sprite links vor das paralax22
			paralax21->setPositionX(paralax22->getPositionX() - paralax21->getContentSize().width);
		}
		if(paralax2->convertToWorldSpace(paralax22->getPosition()).x - paralax22->getContentSize().width/2 >= Director::getInstance()->getVisibleSize().width){
			//paralax22 rechts außerhalb vom sichtbaren Bereich
			//Verschiebe paralax22 Sprite links vor das paralax21
			paralax22->setPositionX(paralax21->getPositionX() - paralax22->getContentSize().width);
		}


		if(paralax1->convertToWorldSpace(paralax11->getPosition()).x - paralax11->getContentSize().width/2 >= Director::getInstance()->getVisibleSize().width){
			//paralax11 rinks außerhalb vom sichtbaren Bereich
			//Verschiebe paralax11 Sprite links vor das paralax12
			paralax11->setPositionX(paralax12->getPositionX() - paralax11->getContentSize().width);
		}
		if(paralax1->convertToWorldSpace(paralax12->getPosition()).x - paralax12->getContentSize().width/2 >= Director::getInstance()->getVisibleSize().width){
			//paralax12 rechts außerhalb vom sichtbaren Bereich
			//Verschiebe paralax12 Sprite links vor das paralax11
			paralax12->setPositionX(paralax11->getPositionX() - paralax12->getContentSize().width);
		}
	}
	*/
}

void GameScene::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
