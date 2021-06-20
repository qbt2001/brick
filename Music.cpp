#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

auto audio = SimpleAudioEngine::getInstance();

void PlayBackgroundMusic() {
	audio->preloadBackgroundMusic("background.mp3");

	//set volume
	audio->setBackgroundMusicVolume(0.5);

	//play background music
	audio->playBackgroundMusic("background.mp3", true);
}
void PauseBackgroundMusic() {
	audio->pauseBackgroundMusic();
}
void PlayEffect() {
	audio->preloadEffect("effect.mp3");

	audio->setEffectsVolume(0.5);

	audio->playEffect("effect.mp3", true);
}