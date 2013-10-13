#ifndef __KONSTANTEN_H__
#define __KONSTANTEN_H__

#include "Box2D\Box2D.h"
#include "cocos2d.h"

#define MATH_PI 3.14159265359f
#define MATH_TO_RADIANS 0.0174532925f

//(1<<0) ist ein Binary Operator
//Weitere hinzufügen durch
//#define NAME (1<<(LetzteZahl+1))
//Ich benutze die Binary sachen zum maskieren
//Ist um einiges schneller als arrays zu durchsuchen oder listen oder so

#define PLAYER (1<<0) // 1
#define ENEMY (1<<1) // 2
#define BULLET (1<<2) // 4
#define PLAYER_FOOT (1<<3) // 8
#define PLAYER_RIGHT_SIDE (1<<4) // 16
#define PLAYER_RIGHT_START_CLIMB (1<<5) // 32
#define PLAYER_LEFT_SIDE (1<<6) // 64
#define PLAYER_LEFT_START_CLIMB (1<<7) // 128
#define CLIMBFIXTURE (1<<8) // 256
#define KISTE (1<<9) // 512
#define ANKER (1<<10) // 1024
#define DYNAMIC_KISTE (1<<11) // 2048

#define ENEMY_FOOT (1<<12) // 4096

#define LEFT_ENEMY_FRONT_FOOT (1<<13) // 8192
#define LEFT_ENEMY_BARRIER_1 (1<<14) // 16xxx
#define LEFT_ENEMY_BARRIER_2 (1<<15) // 32xxx
#define LEFT_ENEMY_OVER_HEAD_1 (1<<16) // 64xxx
#define LEFT_ENEMY_OVER_HEAD_2 (1<<17) // 128xxx
#define LEFT_ENEMY_OVER_HEAD_3 (1<<18) // 256xxx

#define RIGHT_ENEMY_FRONT_FOOT (1<<19) // 512xxx
#define RIGHT_ENEMY_BARRIER_1 (1<<20) // 1024xxx
#define RIGHT_ENEMY_BARRIER_2 (1<<21) // 2048xxx
#define RIGHT_ENEMY_OVER_HEAD_1 (1<<22) // 4096xxx
#define RIGHT_ENEMY_OVER_HEAD_2 (1<<23) // 8192xxx
#define RIGHT_ENEMY_OVER_HEAD_3 (1<<24) // 16xxxxxx

#define MID_ENEMY_OVER_HEAD_1 (1<<25) // 32xxxxxx
#define MID_ENEMY_OVER_HEAD_2 (1<<26) // 64xxxxxx
#define MID_ENEMY_OVER_HEAD_3 (1<<27) // 128xxxxxx


#define ALL_COLLIDING KISTE | CLIMBFIXTURE | DYNAMIC_KISTE

#define BOX2D_STEP 0.015f

#define BOX2D_DEBUG true

//Umrechnung von Box2D Metern (Box2D Einheit) zu CoCos2D Points (Pixeln)
//Minimum: 3.2px/PTM_RATIO = 0.1 Meter
//Maximum: 320px/PTM_RATIO = 10 Meter
#define PTM_RATIO 32.0f

#define GRAVITY_Y 25.0f

//Das hier ist schon mim MapEditor abgestimmt!
#define MIN_KAMERA_POS 0
#define MAX_KAMERA_POS 480
#define MAX_MAP_HEIGHT 1280

//Ja die Speed halt
#define ANKER_SPEED 30.0f

#define NORMALIZE_ANKER_PULL false

#define CAN_JUMP_WHILE_HOOKING true
#define CAN_MOVE_WHILE_HOOKING true
#define CAN_SHOOT_WHILE_HOOKING true

#define PLAYER_SLOW_HOOKING 0.6f

//Bullets

//Wichtig, Gewicht = Kraft die später auf andere Körper übertragen wird
#define BULLET_DENSITY 75.0f

//Ja die Speed halt
#define BULLET_SPEED 35.0f

//In sekunden
#define BULLET_SHOOT_SPEED 0.2f

//So Zeug wie maximale Geschwindigkeit \
//Das man geduckt langsamer läuft
//Kletter-Geschwindigkeit
#pragma region PLAYER_VARIABLES

#define PLAYER_DENITY 10.0f

//Maximale Geschwindigkeit (Wird mit Gewicht multipliziert)
#define PLAYER_MAX_SPEED 0.6f

//Geschwindigkeits-Änderung, also wie schnell man die Richtung wechselt etc.
#define PLAYER_SPEED_CHANGE 2.0f

//Slow wenn der Spieler sich nicht bewegt (Simuliert Reibung)
#define PLAYER_SLOW_MULTIPLIER 0.2f

//Sprung Kraft des Spielers
#define PLAYER_JUMP_SPEED 0.4f

#define PLAYER_START_JUMP_TIME 0.00f

//Größere Zahl weniger Slow (1.0 = bleibt wie aufm Boden)
#define PLAYER_SLOW_FALLING 0.5f
#define PLAYER_SLOW_FLYING 0.6f

#define PLAYER_SLOW_WHILE_FLYING 0.2f

//Slowness, wenn er abspringt
#define PLAYER_SLOW_ON_JUMP 0.4f

#define PLAYER_JUMP_COOLDOWN 0.4f

//Kletter Geschwindigkeit
#define PLAYER_CLIMBING_SPEED 0.4f

#define PLAYER_CLIMBING_START_JUMP 5.0f

//Wie stark bewegbare Objecte vom Spieler weggedrückt werden
#define PLAYER_MAGNETIK_OBJECT_REJECT 5.0f

#pragma endregion

#pragma region PLAYER_FALL_VARIABLES


//Normal Jump 0,45sec

//Fallzeit * this = Zeit in Sekunden für den Debuff (Langsamer Laufen)
#define FALL_DEBUF_TIME_MULTIPLIER 0.0f
//MaxSpeed * this = Maximale Geschwindigkeit die der Spieler noch haben kann mit Debuff (Langsamer Laufen)
#define FALL_DEBUF_SPEED_MULTIPLIER 0.4f
//Nicht zu klein einstellen, sonst schwebt der Spieler nur noch rum
#define FALL_MAX_SPEED_WITHOUT_DEBUFF 0.4f

//Fall-Geschwindigkeit ab wann der Spieler schaden bekommt und stehen bleibt
//3 ist ZIEMLICH schnell
#define FALL_MAX_SPEED_WITHOUT_DAMAGE 0.7f
//FallZeit * this = Damage
#define FALL_DAMAGE_MULTIPLIER 2.0f
//Fallzeit * this = Zeit in Sekunden die der Spieler stehen bleibt;
#define FALL_STOP_TIME_MULTIPLIER 0.0f

#pragma endregion

enum EffectSizes {
	IMPULSE_SIZE =          24
};

enum EntityGroups {
	SHIP =				0x0001,
	ASTEROID =			0x0002,
	IMPULSE =			0x0008
};

struct MyContact {
	b2Fixture *fixtureA;
	b2Fixture *fixtureB;
	bool operator==(const MyContact& other) const
	{
		return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
	}
};

//Bullet, HitBody, Point, Force
struct BulletHit {
	b2Fixture *bulletFixture;
	b2Fixture *hitFixture;

	bool operator==(const BulletHit& other) const
	{
		//Just for better readability
		return (\
			bulletFixture == other.bulletFixture) \
			&& (hitFixture == other.hitFixture);
	}
};

struct ContactCounter{
	int mCount;
	int mFixtureMask;
};

class Konstanten{
public:

	//Create new Animation und speichern in Cache
	//Get Animation:
	//CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName(NAME))
	static float registerAction(char *spriteFrames[], float delay, char*name, bool goBack, bool cache){
		cocos2d::Animation * anim;

		cocos2d::Array * array = new cocos2d::Array();
		anim = cocos2d::Animation::create();

		for(int i=0;spriteFrames[i] != NULL; i++){
			if(cache){
				anim->addSpriteFrame(cocos2d::SpriteFrameCache::getInstance()->spriteFrameByName(spriteFrames[i]));
			}else{
				anim->addSpriteFrameWithFileName(spriteFrames[i]);
			}
		}

		anim->setDelayPerUnit(delay);
		anim->setRestoreOriginalFrame(goBack);

		cocos2d::AnimationCache::getInstance()->addAnimation(anim,name);

		return anim->getDuration();
	};
};



#endif