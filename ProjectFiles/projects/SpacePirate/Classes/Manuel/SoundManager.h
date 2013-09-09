#ifndef  _SOUNDMANAGER_H_
#define  _SOUNDMANAGER_H_

#include "irrKlang.h"

using namespace irrklang;

enum SchussSoundss{
	LASER_GUN = 0,
	GRENADE = 1
};

enum SchreiSoundss{
	LASER_GUN2 = 0,
	GRENADE2 = 1
};

class SoundManager{
private:
	static ISoundEngine* engine;
public:



	static void playBackgroundMusic();
	static void playLaserGun();
	static void playSlice();

	static void playSound(SchussSoundss id);

	SoundManager();
};
#endif