#include "SoundManager.h"

static SoundManager* instance;

static SoundManager* getInstance(){
	if(!instance){
		instance = new SoundManager();
	}
	return instance;
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