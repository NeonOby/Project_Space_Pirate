#ifndef  _SOUNDMANAGER_H_
#define  _SOUNDMANAGER_H_

#include "irrKlang.h"

using namespace irrklang;

enum SchussSoundss{
	LASER_GUN = 0,
	JUMP = 1
};

enum Musics{
	JASON = 0,
	COMPLEX = 1
};

class SoundManager{
private:
	ISoundEngine* engine;
public:

	static void playSound(SchussSoundss id);
	static void playMusic(Musics id);

	SoundManager();
};
#endif