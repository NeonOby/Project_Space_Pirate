#ifndef __KONSTANTEN_H__
#define __KONSTANTEN_H__

//Umrechnung von Box2D Metern (Box2D Einheit) zu CoCos2D Points (Pixeln)
#define PTM_RATIO 64.0f

#pragma region PLAYER_VARIABLES

#define PLAYER_MAX_SPEED 6.0f

#pragma endregion	Maximale Geschwindigkeit \
					Multiplier wenn geduckt \
					Ja

//Fallzeit * this = Zeit in Sekunden für den Debuff (Langsamer Laufen)
#define FALL_DEBUF_TIME_MULTIPLIER 0.6f
//MaxSpeed * this = Maximale Geschwindigkeit die der Spieler noch haben kann mit Debuff (Langsamer Laufen)
#define FALL_DEBUF_SPEED_MULTIPLIER  0.5f

//Fall-Zeit in Sekunden ab wann der Spieler schaden bekommt und stehen bleibt
#define FALL_TIME_MAX_WITHOUT_DAMAGE 0.5f
//FallZeit * this = Damage
#define FALL_DAMAGE_MULTIPLIER 2.0f
//Fallzeit * this = Zeit in Sekunden die der Spieler stehen bleibt;
#define FALL_STOP_TIME_MULTIPLIER 0.15f

enum EffectSizes {
    IMPULSE_SIZE =          24
};

enum EntityGroups {
    SHIP =				0x0001,
    ASTEROID =			0x0002,
    ENEMY =				0x0004,
    IMPULSE =			0x0008
};

class Konstanten{
};

#endif