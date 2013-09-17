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
	static void playMusic(const Musics id);
	static void playMusic(const Musics id, const float position);
	static void playMusic(const Musics id, const float position, ISound** pReturn);
	static void setMusicPos(ISound* steroMusic, const float newPosition);
	static void playSound(const Soundss id);
	static void playSound(const Soundss id, const bool echo);
	static void playSound(const Soundss id, const float position);
};
#endif