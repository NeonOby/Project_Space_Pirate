#ifndef  _SOUNDMANAGER_H_
#define  _SOUNDMANAGER_H_

#include "irrKlang.h"

using namespace irrklang;

class SoundManager{
private:
	ISoundEngine* engine;
public:
	static void playBackgroundMusic();
	static void playLaserGun();
	static void playSlice();

	static void playSound(SchussSoundss id);

	SoundManager();
};
#endif