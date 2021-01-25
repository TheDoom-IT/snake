#include "Audio.h"

Audio::Audio()
{
	mMusicVolume = mSoundVolume = 50;
	//mMusicVolume = 0;
	//mSoundVolume = 10;
}

void Audio::mAudioUpdateVolume(float musicVolume, float soundVolume)
{
	mMusicVolume = musicVolume;
	mSoundVolume = soundVolume;

	if (mMusicVolume < 0) mMusicVolume = 0;
	if (mSoundVolume < 0) mSoundVolume = 0;
	if (mMusicVolume > 100) mMusicVolume = 100;
	if (mSoundVolume > 100) mSoundVolume = 100;

	for (auto x = mMusic.begin(); x != mMusic.end(); x++)
		x->second.setVolume(mMusicVolume);

	for (auto x = mSound.begin(); x != mSound.end(); x++)
		x->second.setVolume(mSoundVolume);
}


void Audio::mAudioStop()
{
	for (auto x = mMusic.begin(); x != mMusic.end(); x++)
		x->second.stop();

	for (auto x = mSound.begin(); x != mSound.end(); x++)
	{
		while (x->second.getStatus() == sf::SoundSource::Status::Playing);
		x->second.stop();
	}

}