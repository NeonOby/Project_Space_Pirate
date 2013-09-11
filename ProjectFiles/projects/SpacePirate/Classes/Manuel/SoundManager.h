#ifndef  _SOUNDMANAGER_H_
#define  _SOUNDMANAGER_H_

#include "irrKlang.h"

using namespace irrklang;

enum Soundss{
	LASER_GUN = 0,
	JUMP = 1
};

enum Musics{
	JASON = 0,
	COMPLEX = 1
};

class SoundManager{
private:

protected:
	

public:
	SoundManager();
	static SoundManager *getInstance();
	static void playMusic(Musics id);
	static void playSound(Soundss id);
	static void playSound(Soundss id, bool echo);
};
#endif