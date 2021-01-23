#ifndef MYGAME_H
#define MYGAME_H
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Audio.h"
#include "Enumerators.h"
#include <string>


//Directions, where snake can move
enum class Move {Left, Right, Up, Down};
//States, which board tile can be in
enum class Board {Empty, Apple, Snake};

//Possible states of game
enum class StateGame {
	MENU, INGAME, GAMEOVER, PAUSE, EXIT
};

struct Cordinate
{
	int x;
	int y;
};

class Game : Audio
{
public:
	//Sprites used in game
	enum class SpriteType {
		Background, Empty, Apple,
		SnakeHeadL,  SnakeHeadU, SnakeHeadR, SnakeHeadD,
		SnakeTailL, SnakeTailU, SnakeTailR,  SnakeTailD,
		SnakeBodyStraightLR1, SnakeBodyStraightLR2,
		SnakeBodyStraightUD1, SnakeBodyStraightUD2,
		SnakeBodyTurnLU, SnakeBodyTurnLD, SnakeBodyTurnRU, SnakeBodyTurnRD
	};

private:
	const int mFPS;

	int mBoardTilesNumber;
	float mBoardSpriteSize;
	float mBoardTileSpriteSize;

	Difficulty mDifficulty;
	BoardSize mBoardSize;

	//Easily allow to change from difficulty to time of snake move
	std::map <Difficulty, sf::Time> mDifficultyToTime;

	//Easily allow to change from BoardSize to number of tiles
	std::map <BoardSize, int> mBoardSizeToNumber;

	
	//Containers for data
	std::map<SpriteType, sf::Texture> mTextures;
	std::map<SpriteType, sf::Sprite> mSprites;
	sf::Texture mTexture;
	std::map<SpriteType, sf::IntRect> mTexturePosition;


	//Window, where whole game takes place
	sf::RenderWindow mWindow;
	//Board (with snake and apples) is rendered here
	sf::RenderTexture mRender;
	sf::Sprite mSpriteBoard;

	//Counts time from last snake's move
	sf::Clock mClock;
	//If this time passes, the snake will move
	sf::Time mMoveTime;
	
	Move mMoveLast;
	Move mMoveNext;

	Board** mBoard;

	//Holds cordinates of snake's parts
	std::vector <Cordinate> mCordSnake;
	Cordinate mCordApple;
	
	//Variables to control straight body animation to obtain slithering effect
	int bodyStateLastUsed;
	//the state of the first body drown after the head (can be 1 or 2)
	int bodyStateFirst;

	unsigned mAppleCounter;

	//Actual game state
	StateGame mState;
	Menu mMenu;

	void mAudioPrepare();

	void mSpritesLoad();
	void mSpritesResize();

	void mPrepareGame();
	void mClearGame();
	
	/* Calculates position in pixels of snake's part or apple for given cordinates
	Draws it in the mRender */
	void mDrawOnBoard(SpriteType, Cordinate);
	void mDrawString(std::string, sf::Color = sf::Color::Red, float y = -1, float x = -1);

	//Place apple somewhere on the board
	void mPlaceApple();
public:
	//Infinite loop of events-update-draw
	void run();
	//Check if any event occurs
	void events();
	//On the basis of events, that took place update game data
	void update();
	//Draw things in mWindow
	void draw();

	Game();
	~Game();
};

#endif