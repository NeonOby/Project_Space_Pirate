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
const int backgroundSequence[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1};
//Das selbe für den midground
const int midground1Sequence[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1};
const int midground2Sequence[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1};

const float speed = 5.0f;

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

	midground2Layer = Layer::create();
	InitSprites(midground2Layer, midground2Sprites, midground2count, MidGround2Sprites, VisibleMidground2Sprites, midground2Sequence);
	this->addChild(midground2Layer, 2);
}


static const float scale = 1.0f;

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
		}

		position = Point(0, Director::getInstance()->getVisibleSize().height/2);
		int lastID = -1;
		int currentID = 0;
		int ImageID = sequence[currentID];
		
		while(ImageID != -1){
			position.x += spriteArray[ImageID]->getContentSize().width/2*scale;
			if(currentID!=0){
				position.x += spriteArray[lastID]->getContentSize().width/2*scale;
			}

			Sprite* tmpSprite = Sprite::createWithTexture(spriteArray[ImageID]->getTexture());
			tmpSprite->setScale(scale);
			tmpSprite->setPosition(position);

			
			layer->addChild(tmpSprite);

			lastID = ImageID;
			currentID++;
			ImageID = sequence[currentID];
		}
		
	}
}



void ParallaxLayer::move(float dt, int direction){
	
	backgroundLayer->setPositionX(backgroundLayer->getPositionX() - dt * direction  * 50.0f);

	midground1Layer->setPositionX(midground1Layer->getPositionX() - dt * direction  * 40.0f);

	//Has to move abit against the move direction !!
	midground2Layer->setPositionX(midground2Layer->getPositionX() - dt * direction  * 30.0f);

}

