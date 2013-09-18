#include "SoundManager.h"

static SoundManager* instance;

const char* Sounds[] = {"Laser.mp3","Sounds/jump01.wav"};
const char* BackgroundMusic[] = {"Music/Jason.mp3","Music/Complex.mp3"};

static SoundManager* getInstance(){
	if(!instance){
		instance = new SoundManager();
	}
	return instance;
}

//Musik und Sounds laden
SoundManager::SoundManager() {
	irrklang::createIrrKlangDevice()->addSoundSourceFromFile("Music/Jason.mp3", irrklang::ESM_AUTO_DETECT, true);
	irrklang::createIrrKlangDevice()->addSoundSourceFromFile("Music/Complex.mp3", irrklang::ESM_AUTO_DETECT, true);
	irrklang::createIrrKlangDevice()->addSoundSourceFromFile("Sounds/jump01.wav", irrklang::ESM_AUTO_DETECT, true);
}

//Musik orginal mittig
void SoundManager::playMusic(const Musics id){
	irrklang::createIrrKlangDevice()->play2D(BackgroundMusic[id], true);
}

//Musik von 1 = links über 0 = mitte bis -1 = rechts abspielen
void SoundManager::playMusic(const Musics id, const float position){
	ISound* stereoMusic = irrklang::createIrrKlangDevice()->play3D(BackgroundMusic[id],vec3df(0,0,position*3), true);
}

//Musik von 1 = links über 0 = mitte bis -1 = rechts abspielen
//Rückgabe: ISound* um Musikposition später ändern zu können
void SoundManager::playMusic(const Musics id, const float position, ISound** pReturn){
	*pReturn = irrklang::createIrrKlangDevice()->play3D(BackgroundMusic[id],vec3df(0, 0, position*3), true, false, true);
}

//neu Position der Musik festlegen mit  1 = links, 0 = mitte oder -1 = rechts abspielen
void SoundManager::setMusicPos(ISound* steroMusic, const float newPosition){
	steroMusic->setPosition(vec3df(0, 0, newPosition*3));
}

//Sound orginal mittig
void SoundManager::playSound(const Soundss id){
	irrklang::createIrrKlangDevice()->play2D(Sounds[id]);
}

//Soundname & true für Echo-Effekt übergeben
void SoundManager::playSound(const Soundss id, const bool echo){
	ISound* effectSound = irrklang::createIrrKlangDevice()->play2D(Sounds[id]);
	if (echo){
		ISoundEffectControl* fx = 0;
		if (effectSound){
			fx = effectSound->getSoundEffectControl();
		}
		if (!fx){
			return;
		}
		fx->enableEchoSoundEffect();
	}
}

//Sound von 1 = links über 0 = mitte bis -1 = rechts abspielen
void SoundManager::playSound(const Soundss id, const float position){
	ISound* stereoSound = irrklang::createIrrKlangDevice()->play3D(Sounds[id],vec3df(0,0,position*3));
}