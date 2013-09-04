#include "SoundManager.h"

static SoundManager* instance;

static SoundManager* getInstance(){
	if(!instance){
		instance = new SoundManager();
	}
	return instance;
}