#include "SoundManager.h"

static SoundManager* instance;




const char* SchussSounds[] = {"Laser.mp3","Granade.mp3","Jason.mp3"};

static SoundManager* getInstance(){
	if(!instance){
		instance = new SoundManager();
	}
	return instance;
}

SoundManager::SoundManager() {
	engine = createIrrKlangDevice();
	engine->addSoundSourceFromFile("Jason.mp3", irrklang::ESM_AUTO_DETECT, true);

	playSound(SchussSoundss::LASER_GUN);
}

void SoundManager::playSound(SchussSoundss id){
	createIrrKlangDevice()->play2D(SchussSounds[id]);
}

void SoundManager::playBackgroundMusic(){
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine){
		return; // error starting up the engine
	}

	engine->play2D("Jason.mp3", true);
	
}

void SoundManager::playLaserGun(){
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine){
		return; // error starting up the engine
	}

	engine->play2D("Sounds/LaserGun01.wav", true);
}

void SoundManager::playSlice(){
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine){
		return; // error starting up the engine
	}

	engine->play2D("Sounds/Slice02.wav",true);
}