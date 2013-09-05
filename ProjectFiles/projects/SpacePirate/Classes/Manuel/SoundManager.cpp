#include "SoundManager.h"

static SoundManager* instance;

static SoundManager* getInstance(){
	if(!instance){
		instance = new SoundManager();
	}
	return instance;
}

static void PlayBackgroundMusic(){
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine){
		return; // error starting up the engine
	}

	engine->play2D("Jason.mp3", true);
}