#ifndef MYAUDIO_H
#define MYAUDIO_H
#include <SFML/Audio.hpp>
#include <map>


//Every Music value represents some music file of the game
enum class Music
{
	MAIN_MENU, INGAME, GAMEOVER
};

//Every Sound value represents some sound file of the game
enum class Sound
{
	CLICK, DRAG, APPLE
};

class Audio
{
protected:
	//Containers working on enumarators Music and Sound
	std::map <Music, sf::Music> mMusic;
	std::map <Sound, sf::SoundBuffer> mSoundBuffer;
	std::map <Sound, sf::Sound> mSound;

	float mMusicVolume;
	float mSoundVolume;

	void mAudioUpdateVolume(float musicVol, float soundVol);
	void mAudioStop();

	void virtual mAudioPrepare() {};
public:
	Audio();
};

#endif