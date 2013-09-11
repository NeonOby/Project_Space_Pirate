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

//Umrechnung von Box2D Metern (Box2D Einheit) zu CoCos2D Points (Pixeln)
#define PTM_RATIO 64.0f

//So Zeug wie maximale Geschwindigkeit \
//Das man geduckt langsamer läuft
//Kletter-Geschwindigkeit
#pragma region PLAYER_VARIABLES

#define PLAYER_DENITY 10.0f

//Maximale Geschwindigkeit (Wird mit Gewicht multipliziert)
#define PLAYER_MAX_SPEED 0.5f

//Geschwindigkeits-Änderung, also wie schnell man die Richtung wechselt etc.
#define PLAYER_SPEED_CHANGE 1.0f

//Sprung Kraft des Spielers
#define PLAYER_JUMP_SPEED 7.0f

#define PLAYER_START_JUMP_TIME 0.00f

//Slowness, wenn er abspringt
#define PLAYER_SLOW_ON_JUMP 0.4f

//Kletter Geschwindigkeit
#define PLAYER_CLIMBING_SPEED 0.3f

#define PLAYER_CLIMBING_START_JUMP 5.0f

#pragma endregion

#pragma region PLAYER_FALL_VARIABLES

//Fallzeit * this = Zeit in Sekunden für den Debuff (Langsamer Laufen)
#define FALL_DEBUF_TIME_MULTIPLIER 0.6f
//MaxSpeed * this = Maximale Geschwindigkeit die der Spieler noch haben kann mit Debuff (Langsamer Laufen)
#define FALL_DEBUF_SPEED_MULTIPLIER 0.5f

//Fall-Zeit in Sekunden ab wann der Spieler schaden bekommt und stehen bleibt
#define FALL_TIME_MAX_WITHOUT_DAMAGE 0.5f
//FallZeit * this = Damage
#define FALL_DAMAGE_MULTIPLIER 2.0f
//Fallzeit * this = Zeit in Sekunden die der Spieler stehen bleibt;
#define FALL_STOP_TIME_MULTIPLIER 0.15f

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