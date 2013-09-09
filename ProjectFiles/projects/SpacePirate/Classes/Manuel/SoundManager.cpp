#include "SoundManager.h"

static SoundManager* instance;

const char* SchussSounds[] = {"Laser.mp3","Sounds/jump01.wav"};
const char* BackgroundMusic[] = {"Music/Jason.mp3","Music/Complex.mp3"};

static SoundManager* getInstance(){
	if(!instance){
		instance = new SoundManager();
	}
	return instance;
}

SoundManager::SoundManager() {
	engine = createIrrKlangDevice();
	engine->addSoundSourceFromFile("Music/Jason.mp3", irrklang::ESM_AUTO_DETECT, true);
	engine->addSoundSourceFromFile("Music/Complex.mp3", irrklang::ESM_AUTO_DETECT, true);
	engine->addSoundSourceFromFile("Sounds/jump01.wav", irrklang::ESM_AUTO_DETECT, true);
	playSound(SchussSoundss::LASER_GUN);
}

void SoundManager::playSound(SchussSoundss id){
	createIrrKlangDevice()->play2D(SchussSounds[id]);
}

void SoundManager::playMusic(Musics id){
	createIrrKlangDevice()->play2D(BackgroundMusic[id]);
}