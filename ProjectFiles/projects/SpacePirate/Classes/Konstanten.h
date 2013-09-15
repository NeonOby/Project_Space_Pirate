#ifndef __KONSTANTEN_H__
#define __KONSTANTEN_H__

#define PLAYER 1
#define ENEMY 2
#define BULLET 3
#define PLAYER_FOOD 4
#define PLAYER_RIGHT_SIDE 5
#define PLAYER_RIGHT_START_CLIMB 6
#define PLAYER_LEFT_SIDE 7
#define PLAYER_LEFT_START_CLIMB 8
#define CLIMBFIXTURE 9
#define KISTE 10

#define BOX2D_STEP 0.02f

#define BOX2D_DEBUG true

//Umrechnung von Box2D Metern (Box2D Einheit) zu CoCos2D Points (Pixeln)
#define PTM_RATIO 64.0f

#define GRAVITY_Y 18.0f

//Das hier ist schon mim MapEditor abgestimmt!
#define MIN_KAMERA_POS 200
#define MAX_KAMERA_POS 280
#define MAX_MAP_HEIGHT 1280

//So Zeug wie maximale Geschwindigkeit \
//Das man geduckt langsamer läuft
//Kletter-Geschwindigkeit
#pragma region PLAYER_VARIABLES

#define PLAYER_DENITY 10.0f

//Maximale Geschwindigkeit (Wird mit Gewicht multipliziert)
#define PLAYER_MAX_SPEED 0.5f

//Geschwindigkeits-Änderung, also wie schnell man die Richtung wechselt etc.
#define PLAYER_SPEED_CHANGE 1.0f

//Slow wenn der Spieler sich nicht bewegt (Simuliert Reibung)
#define PLAYER_SLOW_MULTIPLIER 2.5f

//Sprung Kraft des Spielers
#define PLAYER_JUMP_SPEED 0.8f

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
#define FALL_DEBUF_TIME_MULTIPLIER 0.7f
//MaxSpeed * this = Maximale Geschwindigkeit die der Spieler noch haben kann mit Debuff (Langsamer Laufen)
#define FALL_DEBUF_SPEED_MULTIPLIER 0.4f
//Nicht zu klein einstellen, sonst schwebt der Spieler nur noch rum
#define FALL_MAX_SPEED_WITHOUT_DEBUFF 0.3f

//Fall-Geschwindigkeit ab wann der Spieler schaden bekommt und stehen bleibt
//3 ist ZIEMLICH schnell
#define FALL_MAX_SPEED_WITHOUT_DAMAGE 0.8f
//FallZeit * this = Damage
#define FALL_DAMAGE_MULTIPLIER 2.0f
//Fallzeit * this = Zeit in Sekunden die der Spieler stehen bleibt;
#define FALL_STOP_TIME_MULTIPLIER 0.5f

#pragma endregion

enum EffectSizes {
	IMPULSE_SIZE =          24
};

enum EntityGroups {
	SHIP =				0x0001,
	ASTEROID =			0x0002,
	IMPULSE =			0x0008
};

class Konstanten{
};

#endif