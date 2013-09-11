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

SoundManager::SoundManager() {
	ISoundEngine* engine = createIrrKlangDevice();
	engine->addSoundSourceFromFile("Music/Jason.mp3", irrklang::ESM_AUTO_DETECT, true);
	engine->addSoundSourceFromFile("Music/Complex.mp3", irrklang::ESM_AUTO_DETECT, true);
	engine->addSoundSourceFromFile("Sounds/jump01.wav", irrklang::ESM_AUTO_DETECT, true);
}

void SoundManager::playMusic(Musics id){
	ISoundEngine* engine = createIrrKlangDevice();
	engine->play2D(BackgroundMusic[id],true);
}

void SoundManager::playSound(Soundss id){
	ISoundEngine* engine = createIrrKlangDevice();
	engine->play2D(Sounds[id]);
}

void SoundManager::playSound(Soundss id, bool echo){
	ISoundEngine* engine = createIrrKlangDevice();
	ISound* effectSound = engine->play2D(Sounds[id]);
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