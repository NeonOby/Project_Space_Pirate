#include "ParallaxLayer.h"
#include "AppDelegate.h"
USING_NS_CC;
 
//Anzahl an Elementen in dem Array
const int backgroundcount = 2;
//Die Parallax Bilder in einem Array, Index der Bilder wird später für MapGeneration verwendet
const char* backgroundSprites[] = {"Level/Jungle/Background.PNG","Level/Jungle/Background.PNG"};
const int midground1count = 2;
const char* midground1Sprites[] = {"Level/Jungle/Midground.PNG","Level/Jungle/Midground.PNG"};
const int midground2count = 2;
const char* midground2Sprites[] = {"Level/Jungle/Midground2.PNG","Level/Jungle/Midground2.PNG"};

//Die Bildsequenz in deren Reihenfolge die Backgrounds angezeigt werden, NULL = von vorne anfangen
const int backgroundSequence[] = {0, 1, NULL};
//Das selbe für den midground
const int midground1Sequence[] = {0, 1, NULL};
const int midground2Sequence[] = {0, 1, NULL};

const float speed = 200.0f;

/*
Test


char* ASTEROID_IMAGES[] = {"Asteroid1.png","Asteroid2.png","Asteroid3.png","Asteroid4.png"};

CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		cache->addSpriteFramesWithFile("Asteroids.plist");

CCSprite* sprite = CCSprite::createWithSpriteFrameName(img);
*/

bool ParallaxLayer::init(){

	if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();

	//Wenn wir die Sprite Sheets fertig haben ändern wir die Layer zu BatchNodes, 
	//was das Rendering unheimlich schneller machen sollte

	//Add SpriteSheet with every Background here and add plist to cache
	/*backgroundLayer = CCSpriteBatchNode::create("Asteroids.png");
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("Asteroids.plist");
	*/

	//Load All Sprites into Memory?

	backgroundLayer = Layer::create();
	InitSprites(backgroundLayer, backgroundSprites, backgroundcount, BackGroundSprites, VisibleBackgroundSprites, backgroundSequence);
	this->addChild(backgroundLayer, 0);

	midground1Layer = Layer::create();
	InitSprites(midground1Layer, midground1Sprites, midground1count, MidGround1Sprites, VisibleMidground1Sprites, midground1Sequence);
	this->addChild(midground1Layer, 1);
}


static const float scale = 0.5f;

//Wir iterieren durch ALLE verfügbaren Sprites und laden sie schonmal in den RAM
//Angezeigt werden aber nur die ersten 2 der Sequenz
void ParallaxLayer::InitSprites(Layer* layer, const char** spriteNames, const int spriteCount, Sprite** spriteArray, Sprite** visibleSpriteArray, const int* sequence){
	Point position = Point(0, Director::getInstance()->getVisibleSize().height/2*scale);
	if(spriteCount==1){
		spriteArray = new Sprite*[2];
		//Lade das eine Sprite und zeige es schon 2 mal
		spriteArray[0] = Sprite::create(spriteNames[0]);
		spriteArray[0]->setScale(scale);
		position.operator+(Point(spriteArray[0]->getContentSize().width/2*scale,0));
		spriteArray[0]->setPosition(position);

		spriteArray[1] = Sprite::create(spriteNames[0]);
		spriteArray[1]->setScale(scale);
		//Jetzige Position + die Hälfte vom letzten Sprite + die Hälfte vom jetzigen
		position.operator+(Point((spriteArray[0]->getContentSize().width/2+spriteArray[1]->getContentSize().width/2)*scale,0));
		spriteArray[1]->setPosition(position);

		visibleSpriteArray[0] = spriteArray[0];
		visibleSpriteArray[1] = spriteArray[1];

		layer->addChild(spriteArray[0]);
		layer->addChild(spriteArray[1]);
	}else{
		spriteArray = new Sprite*[spriteCount];
		for(int i=0;i<spriteCount;i++){
			spriteArray[i] = Sprite::create(spriteNames[i]);
			spriteArray[i]->setScale(scale);
			if(i==0)
				position.operator+(Point(spriteArray[i]->getContentSize().width/2*scale,0));
			else
				position.operator+(Point((spriteArray[i-1]->getContentSize().width/2+spriteArray[i]->getContentSize().width/2)*scale,0));

			spriteArray[i]->setPosition(position);

			//Den ersten und zweiten Sprite aus der Sequenz können wir gleich hinzufügen
			//D.h. nicht wenn i = 0,1. Sondern wenn die Sequenz stimmt
			if(i==sequence[0]){
				visibleSpriteArray[0] = spriteArray[i];
				layer->addChild(spriteArray[i]);
			}
			if(i==sequence[1]){
				visibleSpriteArray[1] = spriteArray[i];
				layer->addChild(spriteArray[i]);
			}
		}
	}
}



void ParallaxLayer::move(float dt, int direction){
	
	backgroundLayer->setPositionX(backgroundLayer->getPositionX() + dt * direction * speed * 0.5f);

	/*
	MidGround1Sprites[0]->setPositionX(MidGround1Sprites[0]->getPositionX() + dt * direction * speed * 0.75f);
	MidGround1Sprites[1]->setPositionX(MidGround1Sprites[1]->getPositionX() + dt * direction * speed * 0.75f);

	MidGround2Sprites[0]->setPositionX(MidGround2Sprites[0]->getPositionX() + dt * direction * speed * 0.9f);
	MidGround2Sprites[1]->setPositionX(MidGround2Sprites[1]->getPositionX() + dt * direction * speed * 0.9f);
	*/
}

