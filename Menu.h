#ifndef MYMENU_H
#define MYMENU_H
#include <SFML/Graphics.hpp>
#include "Audio.h"
#include "Enumerators.h"

//Possible states of menu
enum class StateMenu
{
	MAIN_MENU, OPTIONS, GAME_PREPARATION
};

enum class StateGame;

class Menu : Audio
{
public:
	//Sprites used in menu
	enum class SpriteType
	{
		MENU_START, MENU_OPTIONS, MENU_EXIT,
		DIFFICULTY, BOARD_SIZE,
		DIFFICULTY_EASY, DIFFICULTY_NORMAL, DIFFICULTY_HARD,
		SIZE_SMALL, SIZE_MEDIUM, SIZE_LARGE,
		POINTER_DIFFICULTY,
		POINTER_BOARD_SIZE,
		PLAY,
		VOLUME_EFFECTS, VOLUME_MUSIC,
		SLIDER_EFFECTS, SLIDER_MUSIC,
		POINTER_EFFECTS, POINTER_MUSIC,
		SNAKE_TITLE, BACKGROUND
	};

private:
	Difficulty mDifficulty;
	BoardSize mBoardSize;
	
	//This sprite will be sent to Game object and be drawn on the screen
	sf::Sprite mSprite;
	//View of the menu is renderer here
	sf::RenderTexture mRender;

	//size of render in pixels
	sf::Vector2u mSize;
	//Texture read from file containing all needed graphics
	sf::Texture mTexture;

	//Buttons have animations
	//std::map holds position(sf::IntRect) in mTexture of every frame of animation
	std::map<SpriteType, std::map<int,sf::IntRect>> mTexturePosition;
	std::map<SpriteType, sf::Sprite> mSprites;


	//Animation
	SpriteType mAnimationSprite;
	int mAnimationState;
	int mAnimationFrames;
	//If true animation will be played from last frame to first one
	bool mAnimationRevert;
	sf::Clock mAnimationClock;
	//Time after which frame changes
	sf::Time mAnimationTime;
	void mAnimationReset(SpriteType newAnimation = SpriteType::MENU_START);
	void mAnimationUpdate();

	void mAudioPrepare();
	//change volume and also sprites connected with audio Button::SLIDER_MUSIC
	void mChangeVolumeAndSprite(bool volumeUp, SpriteType slider, float currentVolume);

	StateMenu mState;

	void mSpritesLoad();
	void mSpritesResize();
	void mSpritesSetPosition();
public:
	void update();
	StateGame events(sf::Event);
	void draw();

	//Sprite can be taken by Game object and be drawn on the screen
	sf::Sprite getSprite() const { return mSprite; }
	void setSize(sf::Vector2u size) { mSize = size; }

	//restore menu to initial state
	void reset();
	void getSettings(Difficulty&, BoardSize&,float& music,float& sound) const;
	Menu(sf::Vector2u size);
	~Menu();
};

#endif