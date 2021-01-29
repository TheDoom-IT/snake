#include "Menu.h"
#include "Game.h"

Menu::Menu(sf::Vector2u size)
{
	mState = StateMenu::MAIN_MENU;
	mAnimationSprite = SpriteType::MENU_START;
	mAnimationState = 1;
	mAnimationFrames = 6;
	mAnimationTime = sf::seconds(0.1);
	mAnimationRevert = false;

	mBoardSize = BoardSize::MEDIUM;
	mDifficulty = Difficulty::NORMAL;

	mSize = size;
	mRender.create(size.x, size.y);
	
	mSpritesLoad();
	mSpritesResize();
	mSpritesSetPosition();

	mAudioPrepare();
	mMusic[Music::MAIN_MENU].play();
}

Menu::~Menu()
{
	//For now nothing is needed here
}

StateGame Menu::events(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (mState == StateMenu::MAIN_MENU)
		{
			if (event.key.code == sf::Keyboard::Space)
			{
				if (mAnimationSprite == SpriteType::MENU_START)
				{
					mState = StateMenu::GAME_PREPARATION;
					mSound[Sound::CLICK].play();
					mAnimationReset(SpriteType::POINTER_DIFFICULTY);
				}
				else if (mAnimationSprite == SpriteType::MENU_OPTIONS)
				{
					mState = StateMenu::OPTIONS;
					mSound[Sound::CLICK].play();
					mAnimationReset(SpriteType::POINTER_MUSIC);
				}
				else//MENU_EXIT
				{
					mSound[Sound::CLICK].play();
					mAnimationReset();
					mAudioStop();
					return StateGame::EXIT;
				}
			}
			else if (event.key.code == sf::Keyboard::Up)
			{
				if (mAnimationSprite != SpriteType::MENU_START)
					mAnimationReset((SpriteType)((int)mAnimationSprite - 1));
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				if (mAnimationSprite != SpriteType::MENU_EXIT)
					mAnimationReset((SpriteType)((int)mAnimationSprite + 1));
			}
		}
		//GAMEPREPARATION
		else if (mState == StateMenu::GAME_PREPARATION)
		{
			if (event.key.code == sf::Keyboard::Space)
			{
				if (mAnimationSprite == SpriteType::PLAY)
				{
					mSound[Sound::CLICK].play();
					mAnimationReset();
					mAudioStop();
					return StateGame::INGAME;
				}
			}
			else if (event.key.code == sf::Keyboard::Up)
			{
				if (mAnimationSprite != SpriteType::POINTER_DIFFICULTY)
					mAnimationReset((SpriteType)((int)mAnimationSprite - 1));
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				if (mAnimationSprite != SpriteType::PLAY)
					mAnimationReset((SpriteType)((int)mAnimationSprite + 1));
			}
			else if (event.key.code == sf::Keyboard::Right)
			{
				if (mAnimationSprite == SpriteType::POINTER_DIFFICULTY)
				{
					if (mDifficulty != Difficulty::HARD)
						mDifficulty = (Difficulty)((int)mDifficulty + 1);
				}
				else if (mAnimationSprite == SpriteType::POINTER_BOARD_SIZE)
					if (mBoardSize != BoardSize::LARGE)
						mBoardSize = (BoardSize)((int)mBoardSize + 1);
			}
			else if (event.key.code == sf::Keyboard::Left)
			{
				if (mAnimationSprite == SpriteType::POINTER_DIFFICULTY)
				{
					if (mDifficulty != Difficulty::EASY)
						mDifficulty = (Difficulty)((int)mDifficulty - 1);
				}
				else if (mAnimationSprite == SpriteType::POINTER_BOARD_SIZE)
					if (mBoardSize != BoardSize::SMALL)
						mBoardSize = (BoardSize)((int)mBoardSize - 1);
			}
			else if (event.key.code == sf::Keyboard::Escape)
			{
				mAnimationReset();
				mState = StateMenu::MAIN_MENU;
			}
		}
		//OPTIONS
		else if (mState == StateMenu::OPTIONS)
		{
			if (event.key.code == sf::Keyboard::Down)
			{
				if (mAnimationSprite == SpriteType::POINTER_MUSIC)
					mAnimationReset(SpriteType::POINTER_EFFECTS);	
			}
			else if (event.key.code == sf::Keyboard::Up)
			{
				if (mAnimationSprite == SpriteType::POINTER_EFFECTS)
					mAnimationReset(SpriteType::POINTER_MUSIC);
			}
			else if (event.key.code == sf::Keyboard::Right)
			{
				if (mAnimationSprite == SpriteType::POINTER_EFFECTS)
				{
					mChangeVolumeAndSprite(true, SpriteType::POINTER_EFFECTS, mSoundVolume);
				}
				else if (mAnimationSprite == SpriteType::POINTER_MUSIC)
				{

					mChangeVolumeAndSprite(true, SpriteType::POINTER_MUSIC, mMusicVolume);
				}
			}
			else if (event.key.code == sf::Keyboard::Left)
			{
				if (mAnimationSprite == SpriteType::POINTER_EFFECTS)
				{
					mChangeVolumeAndSprite(false, SpriteType::POINTER_EFFECTS, mSoundVolume);
				}
				else if (mAnimationSprite == SpriteType::POINTER_MUSIC)
				{
					mChangeVolumeAndSprite(false, SpriteType::POINTER_MUSIC, mMusicVolume);
				}
			}
			else if (event.key.code == sf::Keyboard::Escape)
			{
				mAnimationReset();
				mState = StateMenu::MAIN_MENU;
			}
		}
	}

	return StateGame::MENU;
}

void Menu::update()
{
	mAnimationUpdate();
}

void Menu::draw()
{	
	mRender.clear();
	
	//BACKGROUND
	for (int x = 0; x <= mSize.x; x += mSprites[SpriteType::BACKGROUND].getGlobalBounds().width)
	{
		for (int y = 0; y <= mSize.y; y += mSprites[SpriteType::BACKGROUND].getGlobalBounds().height)
		{
			mSprites[SpriteType::BACKGROUND].setPosition(x + mSprites[SpriteType::BACKGROUND].getGlobalBounds().width/2.0, y + mSprites[SpriteType::BACKGROUND].getGlobalBounds().height / 2.0);
			mRender.draw(mSprites[SpriteType::BACKGROUND]);
		}
	}

	if (mState == StateMenu::MAIN_MENU)
	{
		for(int x = 0; x <= 2; x++)
		mRender.draw(mSprites[(SpriteType)x]);

		mRender.draw(mSprites[SpriteType::SNAKE_TITLE]);
	}
	else if (mState == StateMenu::GAME_PREPARATION)
	{
		mRender.draw(mSprites[SpriteType::DIFFICULTY]);
		mRender.draw(mSprites[SpriteType::BOARD_SIZE]);

		//DIFFICULTY_CHOOSE OR BOARD_SIZE_CHOOSE
		if(mAnimationSprite != SpriteType::PLAY)
		  mRender.draw(mSprites[mAnimationSprite]);

		mRender.draw(mSprites[SpriteType::PLAY]);

		mRender.draw(mSprites[(SpriteType)mDifficulty]);
		mRender.draw(mSprites[(SpriteType)mBoardSize]);
	}
	else if (mState == StateMenu::OPTIONS)
		for (int x = 14; x <= 19; ++x)
			mRender.draw(mSprites[(SpriteType)x]);

	mRender.display();
	mSprite.setTexture(mRender.getTexture(), true);
}

void Menu::mAnimationReset(SpriteType newAnimation)
{
	mSprites[mAnimationSprite].setTextureRect(mTexturePosition[mAnimationSprite][1]);
	mAnimationRevert = false;
	mAnimationState = 1;
	mAnimationClock.restart();
	mAnimationSprite = newAnimation;
}

void Menu::mAnimationUpdate()
{
	if (mAnimationClock.getElapsedTime() > mAnimationTime)
	{
		if (mAnimationRevert == false)
		{
			if (mAnimationState != mAnimationFrames)
				mAnimationState++;
			else
			{
				--mAnimationState;
				mAnimationRevert = true;
			}
		}
		else
		{
			if (mAnimationState != 1)
				mAnimationState--;
			else
			{
				++mAnimationState;
				mAnimationRevert = false;
			}
		}

		mAnimationClock.restart();
	}

	mSprites[mAnimationSprite].setTextureRect(mTexturePosition[mAnimationSprite][mAnimationState]);
}

void Menu::mSpritesResize()
{
	sf::Vector2f scale;

	//all buttons have the same dimensions in the .png
	scale.y = scale.x = 1 / 6.0 * mSize.y / mSprites[SpriteType::MENU_START].getLocalBounds().height;
	for (int x = 0; x <= 4; x++)
		mSprites[(SpriteType)x].setScale(scale);
	
	for(int x = 13; x <= 15; x++)
		mSprites[(SpriteType)x].setScale(scale);

	mSprites[SpriteType::BACKGROUND].setScale(scale);

	scale.y = scale.x = scale.y * 2;
	mSprites[SpriteType::SNAKE_TITLE].setScale(scale);

	scale.y = scale.x = scale.y * 3 / 8.0;
	for (int x = 5; x <= 12; x++)
		mSprites[(SpriteType)x].setScale(scale);

	
	mSprites[SpriteType::SLIDER_EFFECTS].setScale(scale);
	mSprites[SpriteType::SLIDER_MUSIC].setScale(scale);

	scale.y = scale.x = 1 / 4.0 * mSprites[SpriteType::SLIDER_EFFECTS].getGlobalBounds().height / mSprites[SpriteType::POINTER_EFFECTS].getLocalBounds().height;
	mSprites[SpriteType::POINTER_EFFECTS].setScale(scale);
	mSprites[SpriteType::POINTER_MUSIC].setScale(scale);
}

void Menu::mSpritesLoad()
{
	mTexture.loadFromFile("Images\\Menu.png");
	//0-10th element of SpriteType
	//loading every frame of animation
	for (int y = 0; y <= 10; y++)
		for (int x = 0; x < mAnimationFrames; x++)
			mTexturePosition[(SpriteType)y][x + 1] = sf::IntRect(1200 * x, 300 * y, 1200, 300);

	//11-13
	for (int x = 0; x < 6; x++)
	{
		mTexturePosition[SpriteType::POINTER_DIFFICULTY][x + 1] = mTexturePosition[SpriteType::POINTER_BOARD_SIZE][x + 1] = sf::IntRect(1500 * x, 3300, 1500, 300);
		mTexturePosition[SpriteType::PLAY][x + 1] = sf::IntRect(1200 * x, 3600, 1200, 300);
	}

	//14-19
	mTexturePosition[SpriteType::VOLUME_EFFECTS][1] = sf::IntRect(0, 3900, 1200, 300);
	mTexturePosition[SpriteType::VOLUME_MUSIC][1] = sf::IntRect(1200, 3900, 1200, 300);
	mTexturePosition[SpriteType::SLIDER_MUSIC][1] = mTexturePosition[SpriteType::SLIDER_EFFECTS][1] = sf::IntRect(0, 4500, 1200, 300);
	for (int x = 0; x < 6; x++)
	{
		mTexturePosition[SpriteType::POINTER_MUSIC][x + 1] = mTexturePosition[SpriteType::POINTER_EFFECTS][x + 1] = sf::IntRect(1200 + 300 * x, 4500, 300, 300);
	}

	//20 SNAKE_STRING
	mTexturePosition[SpriteType::SNAKE_TITLE][1] = sf::IntRect(0, 4200, 1200, 300);

	//21 SNAKE BACKGROUND
	mTexturePosition[SpriteType::BACKGROUND][1] = sf::IntRect(1200, 4200, 300, 300);
	
	//assigns every sprite its first, basic frame
	for (int x = 0; x <= 21; x++)
	{
		mSprites[(SpriteType)x].setTexture(mTexture);
		mSprites[(SpriteType)x].setTextureRect(mTexturePosition[(SpriteType)x][1]);
		mSprites[(SpriteType)x].setOrigin(mSprites[(SpriteType)x].getLocalBounds().width / 2.0, mSprites[(SpriteType)x].getLocalBounds().height / 2.0);
	}
}

void Menu::mSpritesSetPosition()
{
	//MAIN_MENU
	for (int x = 0; x < 3; x++)
		mSprites[(SpriteType)x].setPosition(mSize.x / 2, mSize.y / 2 + mSprites[(SpriteType)x].getGlobalBounds().height * x);
	//game title
	mSprites[SpriteType::SNAKE_TITLE].setPosition(mSize.x / 2, mSize.y / 2 - mSprites[SpriteType::SNAKE_TITLE].getGlobalBounds().height);

	//GAME_PREPARATION
	mSprites[SpriteType::DIFFICULTY].setPosition(mSize.x / 2, mSize.y / 3.0);
	mSprites[SpriteType::BOARD_SIZE].setPosition(mSprites[SpriteType::DIFFICULTY].getPosition());
	mSprites[SpriteType::BOARD_SIZE].move(0, 1 / 4.0 * mSize.y);
	
	mSprites[SpriteType::DIFFICULTY_EASY].setPosition(mSprites[SpriteType::DIFFICULTY].getPosition());
	mSprites[SpriteType::DIFFICULTY_EASY].move(0, 1 / 12.0 * mSize.y);
	for (int x = 6; x <= 7; x++)
		mSprites[(SpriteType)x].setPosition(mSprites[SpriteType::DIFFICULTY_EASY].getPosition());

	mSprites[SpriteType::SIZE_SMALL].setPosition(mSprites[SpriteType::BOARD_SIZE].getPosition());
	mSprites[SpriteType::SIZE_SMALL].move(0, 1 / 12.0 * mSize.y);
	for (int x = 9; x <= 10; x++)
		mSprites[(SpriteType)x].setPosition(mSprites[SpriteType::SIZE_SMALL].getPosition());

	mSprites[SpriteType::POINTER_DIFFICULTY].setPosition(mSprites[SpriteType::DIFFICULTY_EASY].getPosition());
	mSprites[SpriteType::POINTER_BOARD_SIZE].setPosition(mSprites[SpriteType::SIZE_SMALL].getPosition());

	mSprites[SpriteType::PLAY].setPosition(mSprites[SpriteType::POINTER_BOARD_SIZE].getPosition());
	mSprites[SpriteType::PLAY].move(0, 1 / 6.0 * mSize.y);

	//OPTIONS
	mSprites[SpriteType::VOLUME_MUSIC].setPosition(mSize.x / 2.0, mSize.y / 3);
	
	mSprites[SpriteType::SLIDER_MUSIC].setPosition(mSprites[SpriteType::VOLUME_MUSIC].getPosition());
	mSprites[SpriteType::SLIDER_MUSIC].move(0, mSize.y / 6.0);

	mSprites[SpriteType::VOLUME_EFFECTS].setPosition(mSprites[SpriteType::SLIDER_MUSIC].getPosition());
	mSprites[SpriteType::VOLUME_EFFECTS].move(0, mSize.y / 6.0);

	mSprites[SpriteType::SLIDER_EFFECTS].setPosition(mSprites[SpriteType::VOLUME_EFFECTS].getPosition());
	mSprites[SpriteType::SLIDER_EFFECTS].move(0, mSize.y / 6.0);

	//move in x direction will set POINTER_EFFECTS to proper position depending on mSoundVolume initial value
	mSprites[SpriteType::POINTER_EFFECTS].setPosition(mSprites[SpriteType::SLIDER_EFFECTS].getPosition());
	mSprites[SpriteType::POINTER_EFFECTS].move(mSprites[SpriteType::SLIDER_EFFECTS].getGlobalBounds().width * 0.75 / 11 * (mSoundVolume - 50)/10,
		 -mSprites[SpriteType::SLIDER_EFFECTS].getGlobalBounds().height / 12.0 * 5);

	mSprites[SpriteType::POINTER_MUSIC].setPosition(mSprites[SpriteType::SLIDER_MUSIC].getPosition());
	mSprites[SpriteType::POINTER_MUSIC].move(mSprites[SpriteType::SLIDER_MUSIC].getGlobalBounds().width * 0.75 / 11 * (mMusicVolume-50)/10,
		 -mSprites[SpriteType::SLIDER_MUSIC].getGlobalBounds().height / 12.0 * 5);
}

void Menu::getSettings (Difficulty& dif, BoardSize& size, float& music, float& sound)const
{
	dif = mDifficulty;
	size = mBoardSize;
	music = mMusicVolume;
	sound = mSoundVolume;
}

void Menu::mAudioPrepare()
{
	mMusic[Music::MAIN_MENU].openFromFile("Sounds\\Main.wav");
	mMusic[Music::MAIN_MENU].setLoop(true);

	mSoundBuffer[Sound::CLICK].loadFromFile("Sounds\\Click.wav");
	mSound[Sound::CLICK].setBuffer(mSoundBuffer[Sound::CLICK]);
	mSoundBuffer[Sound::DRAG].loadFromFile("Sounds\\Drag.wav");
	mSound[Sound::DRAG].setBuffer(mSoundBuffer[Sound::DRAG]);

	mAudioUpdateVolume(mMusicVolume, mSoundVolume);
} 

void Menu::mChangeVolumeAndSprite(bool volumeUp, SpriteType pointer, float currentVolume)
{
	//both sliders has the same dimensions, we can use whichever we want to calculate move
	float moveX = mSprites[SpriteType::SLIDER_MUSIC].getGlobalBounds().width * 0.75/11;
	float toAdd = 10;

	if (volumeUp)
	{
		if (currentVolume < 100)
			mSprites[pointer].move(moveX, 0);
	}
	else
	{
		if (currentVolume > 0)
			mSprites[pointer].move(-moveX, 0);
		toAdd *= -1;
	}

	if (pointer == SpriteType::POINTER_EFFECTS)
		mAudioUpdateVolume(mMusicVolume, mSoundVolume + toAdd);
	else
		mAudioUpdateVolume(mMusicVolume + toAdd, mSoundVolume);

	mSound[Sound::DRAG].play();
}

void Menu::reset()
{
	mState = StateMenu::MAIN_MENU;
	mMusic[Music::MAIN_MENU].play();
}