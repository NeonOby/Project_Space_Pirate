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
	ISoundEngine* engine = createIrrKlangDevice();
	engine->addSoundSourceFromFile("Music/Jason.mp3", irrklang::ESM_AUTO_DETECT, true);
	engine->addSoundSourceFromFile("Music/Complex.mp3", irrklang::ESM_AUTO_DETECT, true);
	engine->addSoundSourceFromFile("Sounds/jump01.wav", irrklang::ESM_AUTO_DETECT, true);
}

//Musik orginal mittig
void SoundManager::playMusic(Musics id){
	ISoundEngine* engine = createIrrKlangDevice();
	engine->play2D(BackgroundMusic[id],true);
}

//Musik von 1 = links über 0 = mitte bis -1 = rechts abspielen
void SoundManager::playMusic(Musics id, float position){
	ISoundEngine* engine = createIrrKlangDevice();
	ISound* stereoMusic = engine->play3D(BackgroundMusic[id],vec3df(0,0,position*3),true);
}

//Musik von 1 = links über 0 = mitte bis -1 = rechts abspielen
//Rückgabe: ISound* um Musikposition später ändern zu können
//ISound* SoundManager::playNReturnMusic(Musics id, float position){
//	ISoundEngine* engine = createIrrKlangDevice();
//	ISound* stereoMusic = engine->play3D(BackgroundMusic[id],vec3df(0,0,position*3),true);
//	return stereoMusic;
//}

//Musik von 1 = links über 0 = mitte bis -1 = rechts abspielen
//Rückgabe: ISound* um Musikposition später ändern zu können
	static void playMusic(Musics id, float position, ISound& retMusic){
	ISoundEngine* engine = createIrrKlangDevice();
	retMusic = *engine->play3D(BackgroundMusic[id],vec3df(0,0,position*3),true);
}

//neu Position der Musik festlegen mit  1 = links, 0 = mitte oder -1 = rechts abspielen
void SoundManager::setMusicPos(ISound* steroMusic, float newPosition){
	steroMusic->setPosition(vec3df(0,0,newPosition*3));
}

//Sound orginal mittig
void SoundManager::playSound(Soundss id){
	ISoundEngine* engine = createIrrKlangDevice();
	engine->play2D(Sounds[id]);
}

//Soundname & true für Echo-Effekt übergeben
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

//Sound von 1 = links über 0 = mitte bis -1 = rechts abspielen
void SoundManager::playSound(Soundss id, float position){
	ISoundEngine* engine = createIrrKlangDevice();
	ISound* stereoSound = engine->play3D(Sounds[id],vec3df(0,0,position*3));
}