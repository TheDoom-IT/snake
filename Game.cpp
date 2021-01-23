#include "Game.h"
#include <ctime>
#include <cstdlib>
#include <iostream>

Game::Game() : mWindow(sf::VideoMode(800, 600), "SFML works!"), mMenu(mWindow.getSize()), mFPS(30),
			   mBoardSpriteSize(600)
{
	mState = StateGame::MENU;
	srand(time(NULL));
	mMoveTime = sf::seconds(0.09);

	mDifficulty = Difficulty::NORMAL;
	mBoardSize = BoardSize::MEDIUM;


	mMoveNext = Move::Right;
	mMoveLast = Move::Left;

	//this values will be updated in mPrepareGame() function
	mBoardTilesNumber = 0;
	mBoardTileSpriteSize = 0;
	mBoard = nullptr;
	
	mDifficultyToTime[Difficulty::EASY] = sf::seconds(0.25);
	mDifficultyToTime[Difficulty::NORMAL] = sf::seconds(0.12);
	mDifficultyToTime[Difficulty::HARD] = sf::seconds(0.05);

	mBoardSizeToNumber[BoardSize::SMALL] = 10;
	mBoardSizeToNumber[BoardSize::MEDIUM] = 25;
	mBoardSizeToNumber[BoardSize::LARGE] = 50;

	mSpritesLoad();
	mAudioPrepare();

	bodyStateFirst = bodyStateLastUsed = 1;
}


Game::~Game()
{
	mClearGame();
}

void Game::run()
{
	sf::Time timePerFrame = sf::seconds(1 / mFPS);

	sf::Clock clockFPS;
	while(mWindow.isOpen())
	{
		events();
		if (clockFPS.getElapsedTime() >= timePerFrame)
		{
			update();
			draw();
			clockFPS.restart();
		}
	}
}

void Game::draw()
{
	if (mState == StateGame::MENU)
	{
		mMenu.draw();
		mWindow.clear();
		mWindow.draw(mMenu.getSprite());
		mWindow.display();
	}
	else if(mState != StateGame::EXIT)
	{
		mRender.clear();
		mWindow.clear();

		//background
		for (int x = 0; x <= mWindow.getSize().x; x += mSprites[SpriteType::Background].getGlobalBounds().width)
		{
			for (int y = 0; y <= mWindow.getSize().y; y += mSprites[SpriteType::Background].getGlobalBounds().height)
			{
				mSprites[SpriteType::Background].setPosition(x, y);
				mWindow.draw(mSprites[SpriteType::Background]);
			}
		}
		//board
		for (int x = 0; x < mBoardTilesNumber; x++)
			for (int y = 0; y < mBoardTilesNumber; y++)
				mDrawOnBoard(SpriteType::Empty, { x,y });

		//snake
		bodyStateLastUsed = bodyStateFirst;
		for (int i = 1; i < mCordSnake.size()-1; i++)
		{
			bodyStateLastUsed = bodyStateLastUsed % 2 + 1;
			//STRAIGHT BODY
			if (mCordSnake[i - 1].y == mCordSnake[i + 1].y)
			{
				if (bodyStateLastUsed == 1)
					mDrawOnBoard(SpriteType::SnakeBodyStraightLR1, mCordSnake[i]);
				else
					mDrawOnBoard(SpriteType::SnakeBodyStraightLR2, mCordSnake[i]);
			}
			else if (mCordSnake[i - 1].x == mCordSnake[i + 1].x)
			{
				if(bodyStateLastUsed == 1)
					mDrawOnBoard(SpriteType::SnakeBodyStraightUD1, mCordSnake[i]);
				else
					mDrawOnBoard(SpriteType::SnakeBodyStraightUD2, mCordSnake[i]);
			}

			//BEND BODY
			else if (mCordSnake[i - 1].y == mCordSnake[i].y)
			{
				//"head" is on the left
				if (mCordSnake[i - 1].x == mCordSnake[i].x - 1)
				{
					if (mCordSnake[i + 1].y == mCordSnake[i].y + 1)
						mDrawOnBoard(SpriteType::SnakeBodyTurnLD, mCordSnake[i]);
					else
						mDrawOnBoard(SpriteType::SnakeBodyTurnLU, mCordSnake[i]);
				}
				else//"head" is on the right
				{
					if (mCordSnake[i + 1].y == mCordSnake[i].y + 1)
						mDrawOnBoard(SpriteType::SnakeBodyTurnRD, mCordSnake[i]);
					else
						mDrawOnBoard(SpriteType::SnakeBodyTurnRU, mCordSnake[i]);
				}
			}
			else
			{
				//"head" is on the top
				if (mCordSnake[i - 1].y == mCordSnake[i].y - 1)
				{
					if (mCordSnake[i + 1].x == mCordSnake[i].x + 1)
						mDrawOnBoard(SpriteType::SnakeBodyTurnRU, mCordSnake[i]);
					else
						mDrawOnBoard(SpriteType::SnakeBodyTurnLU, mCordSnake[i]);
				}
				else//"head" is on the bottom
				{
					if (mCordSnake[i + 1].x == mCordSnake[i].x + 1)
						mDrawOnBoard(SpriteType::SnakeBodyTurnRD, mCordSnake[i]);
					else
						mDrawOnBoard(SpriteType::SnakeBodyTurnLD, mCordSnake[i]);
				}
			}
		}

		//SNAKE'S HEAD
		if (mCordSnake[1].x == mCordSnake[0].x + 1)
			mDrawOnBoard(SpriteType::SnakeHeadR, mCordSnake[0]);
		else if (mCordSnake[1].x == mCordSnake[0].x - 1)
			mDrawOnBoard(SpriteType::SnakeHeadL, mCordSnake[0]);
		else if (mCordSnake[1].y == mCordSnake[0].y + 1)
			mDrawOnBoard(SpriteType::SnakeHeadD, mCordSnake[0]);
		else if (mCordSnake[1].y == mCordSnake[0].y - 1)
			mDrawOnBoard(SpriteType::SnakeHeadU, mCordSnake[0]);

		//SNAKE'S TAIL
		if (mCordSnake[mCordSnake.size() - 2].y == mCordSnake[mCordSnake.size() - 1].y + 1)
			mDrawOnBoard(SpriteType::SnakeTailD, mCordSnake[mCordSnake.size() - 1]);
		else if (mCordSnake[mCordSnake.size() - 2].y == mCordSnake[mCordSnake.size() - 1].y - 1)
			mDrawOnBoard(SpriteType::SnakeTailU, mCordSnake[mCordSnake.size() - 1]);
		else if (mCordSnake[mCordSnake.size() - 2].x == mCordSnake[mCordSnake.size() - 1].x + 1)
			mDrawOnBoard(SpriteType::SnakeTailR, mCordSnake[mCordSnake.size() - 1]);
		else mDrawOnBoard(SpriteType::SnakeTailL, mCordSnake[mCordSnake.size() - 1]);

		//apple
		mDrawOnBoard(SpriteType::Apple, mCordApple);


		mRender.display();
		mSpriteBoard.setTexture(mRender.getTexture(), true);
		mSpriteBoard.setOrigin(mSpriteBoard.getLocalBounds().width / 2.0, mSpriteBoard.getLocalBounds().height / 2.0);
		mSpriteBoard.setPosition(mWindow.getSize().x / 2.0, mWindow.getSize().y / 2.0);
		mWindow.draw(mSpriteBoard);


		//apple counter
		sf::Sprite apple;
		apple.setTexture(mTexture);
		apple.setTextureRect(mTexturePosition[SpriteType::Apple]);
		apple.setOrigin(apple.getLocalBounds().width / 2.0, apple.getLocalBounds().height / 2.0);
		float factor = (mWindow.getSize().x - mRender.getSize().x) / 2.0 / apple.getGlobalBounds().width;
		apple.setScale(factor,factor);
		apple.setPosition((mWindow.getSize().x - mRender.getSize().x) / 4.0, apple.getGlobalBounds().height/2.0);
		mWindow.draw(apple);

		mDrawString(std::to_string(mAppleCounter), sf::Color::Red, 
					apple.getPosition().y + apple.getGlobalBounds().height/2.0, apple.getPosition().x);

		if (mState == StateGame::GAMEOVER)
		{
			mDrawString("Game Over\nPress Enter to start again...\nPress escape to back to main menu...");
		}
		else if (mState == StateGame::PAUSE)
		{
			mDrawString("Pause\nPress enter to continue...", sf::Color::Red);
		}

		
		mWindow.display();
	}
}

void Game::update()
{
	if (mState == StateGame::EXIT)
	{
		mWindow.close();
	}
	else if (mState == StateGame::MENU)
	{
		mMenu.update();
	}
	else if (mState == StateGame::INGAME)
	{
		if (mClock.getElapsedTime() >= mMoveTime)
		{
			//as the snake moved, the body animation need to change
			bodyStateFirst = bodyStateFirst % 2 + 1;

			Cordinate tail = mCordSnake[mCordSnake.size() - 1];
			mBoard[tail.x][tail.y] = Board::Empty;
			for (int i = mCordSnake.size() - 1; i >= 1; i--)
			{
				mCordSnake[i] = mCordSnake[i - 1];
			}

			if (mMoveNext == Move::Right)
			{
				mCordSnake[0].x++;
				mMoveLast = Move::Right;
			}
			else if (mMoveNext == Move::Left)
			{
				mCordSnake[0].x--;
				mMoveLast = Move::Left;
			}
			else if (mMoveNext == Move::Up)
			{
				mCordSnake[0].y--;
				mMoveLast = Move::Up;
			}
			else if (mMoveNext == Move::Down)
			{
				mCordSnake[0].y++;
				mMoveLast = Move::Down;
			}

			//check if player don't lose
			if (mCordSnake[0].x >= mBoardTilesNumber || mCordSnake[0].x < 0 || mCordSnake[0].y < 0 ||
					mCordSnake[0].y >= mBoardTilesNumber || mBoard[mCordSnake[0].x][mCordSnake[0].y] == Board::Snake)
			{
				mMusic[Music::GAMEOVER].stop();
				mMusic[Music::GAMEOVER].play();
				mState = StateGame::GAMEOVER;
				std::cout << "GameOver";
			}
			else if (mBoard[mCordSnake[0].x][mCordSnake[0].y] == Board::Apple)
			{
				mSound[Sound::APPLE].play();

				mCordSnake.push_back(tail);
				mBoard[mCordSnake[0].x][mCordSnake[0].y] = Board::Snake;
				mBoard[tail.x][tail.y] = Board::Snake;

				mPlaceApple();
				mAppleCounter++;
			}
			else
				mBoard[mCordSnake[0].x][mCordSnake[0].y] = Board::Snake;


			mClock.restart();
		}
	}
}

void Game::events()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mState = StateGame::EXIT;
		if (event.type == sf::Event::KeyPressed)
		{
			if (mState == StateGame::INGAME)
			{
				if (event.key.code == sf::Keyboard::Key::Left)
				{
					if (mMoveLast != Move::Right)
						mMoveNext = Move::Left;
				}
				else if (event.key.code == sf::Keyboard::Key::Right)
				{
					if (mMoveLast != Move::Left)
						mMoveNext = Move::Right;
				}
				else if (event.key.code == sf::Keyboard::Key::Up)
				{
					if (mMoveLast != Move::Down)
						mMoveNext = Move::Up;
				}
				else if (event.key.code == sf::Keyboard::Key::Down)
				{
					if (mMoveLast != Move::Up)
						mMoveNext = Move::Down;
				}
				else if (event.key.code == sf::Keyboard::Key::Escape)
				{
					mState = StateGame::PAUSE;
				}
			}
			else if (mState == StateGame::PAUSE)
			{
				if (event.key.code == sf::Keyboard::Key::Enter)
				{
					mState = StateGame::INGAME;
					mClock.restart();
				}
			}
			else if (mState == StateGame::GAMEOVER)
			{
				if (event.key.code == sf::Keyboard::Key::Enter)
				{
					mClearGame();
					mPrepareGame();
					mState = StateGame::PAUSE;
				}
				else if (event.key.code == sf::Keyboard::Key::Escape)
				{
					mClearGame();
					mAudioStop();
					mMenu.reset();
					mState = StateGame::MENU;
				}
			}
			else//MENU
			{
				mState = mMenu.events(event);
				//MENU has been closed and now we can start a game
				if (mState == StateGame::INGAME)
				{
					mMenu.getSettings(mDifficulty, mBoardSize,mMusicVolume, mSoundVolume);
					mPrepareGame();
				}
			}
				
		}
	}
}

void Game::mSpritesLoad()
{
	mTexture.loadFromFile("Images\\Game.png");

	mTexturePosition[SpriteType::Background] = sf::IntRect(0, 0, 150, 150);
	//empty, apple
	for (int x = 1; x <= 2; ++x)
	{
		mTexturePosition[(SpriteType)x] = sf::IntRect(150 * x, 0, 150, 150);
	}
	//snake head
	for (int x = 3; x <= 6; ++x)
	{
		mTexturePosition[(SpriteType)x] = sf::IntRect(750, 0, 150, 150);
	}
	//snake tail
	for (int x = 7; x <= 10; ++x)
	{
		mTexturePosition[(SpriteType)x] = sf::IntRect(600, 150, 150, 150);
	}
	//snake straight
	mTexturePosition[SpriteType::SnakeBodyStraightLR1]
		= mTexturePosition[SpriteType::SnakeBodyStraightUD1] = sf::IntRect(450, 0, 150, 150);

	mTexturePosition[SpriteType::SnakeBodyStraightLR2]
		= mTexturePosition[SpriteType::SnakeBodyStraightUD2] = sf::IntRect(600, 0, 150, 150);

	for (int x = 15; x <= 18; x++)
	{
		mTexturePosition[(SpriteType)x] = sf::IntRect(150 * (x-15), 150, 150, 150);
	}

	for (int x = 0; x <= 18; x++)
	{
		mSprites[(SpriteType)x].setTexture(mTexture);
		mSprites[(SpriteType)x].setTextureRect(mTexturePosition[(SpriteType)x]);
		mSprites[(SpriteType)x].setOrigin(mSprites[(SpriteType)x].getLocalBounds().width / 2.0, mSprites[(SpriteType)x].getLocalBounds().height / 2.0);
	}

	//snake head rotation
	for (int x = 4; x <= 6; ++x)
	{
		mSprites[(SpriteType)x].setRotation(90 * (x - 3));
	}

	//snake tail rotation
	for (int x = 8; x <= 10; ++x)
	{
		mSprites[(SpriteType)x].setRotation(90 * (x - 7));
	}

	//straight body rotation
	for (int x = 13; x <= 14; ++x)
	{
		mSprites[(SpriteType)x].setRotation(90);
	}
}

void Game::mSpritesResize()
{
	sf::Vector2f scale;
	//everything except background is resized
	for (auto x = ++mSprites.begin(); x != mSprites.end(); x++)
	{
		scale.x = scale.y = mBoardTileSpriteSize / x->second.getLocalBounds().width;
		x->second.setScale(scale);
	}
}

void Game::mDrawOnBoard(SpriteType type, Cordinate cordinates)
{
	sf::Vector2f position;
	position.x = mBoardTileSpriteSize * (cordinates.x + 0.5);
	position.y = mBoardTileSpriteSize * (cordinates.y + 0.5);
	mSprites[type].setPosition(position);
	mRender.draw(mSprites[type]);
}

void Game::mPlaceApple()
{
	int x, y;
	x = rand() % mBoardTilesNumber;
	y = rand() % mBoardTilesNumber;

	//if random place is occupied by snake it looks for the nearest empty place
	while (mBoard[x][y] != Board::Empty)
	{
		++y;
		if (y >= mBoardTilesNumber)
		{
			++x;
			y = 0;
		}
		if (x >= mBoardTilesNumber)
			x = 0;
	}

	mBoard[x][y] = Board::Apple;
	mCordApple.x = x;
	mCordApple.y = y;
}

void Game::mPrepareGame()
{
	mMoveTime = mDifficultyToTime[mDifficulty];
	mBoardTilesNumber = mBoardSizeToNumber[mBoardSize];
	mBoardTileSpriteSize = mBoardSpriteSize / (float)mBoardTilesNumber;
	mSpritesResize();

	mAudioUpdateVolume(mMusicVolume, mSoundVolume);

	mBoard = new Board* [mBoardTilesNumber];
	for (int x = 0; x < mBoardTilesNumber; x++)
		mBoard[x] = new Board[mBoardTilesNumber];

	for (int x = 0; x < mBoardTilesNumber; x++)
		for (int y = 0; y < mBoardTilesNumber; y++)
			mBoard[x][y] = Board::Empty;

	mMoveLast = Move::Right;
	mMoveNext = Move::Right;

	//Starting position of snake
	mBoard[0][0] = mBoard[1][0] = mBoard[2][0] = Board::Snake;
	mCordSnake.push_back({ 2,0 });
	mCordSnake.push_back({ 1,0 });
	mCordSnake.push_back({ 0,0 });

	mAppleCounter = 0;
	mPlaceApple();
	mRender.create(mBoardSpriteSize, mBoardSpriteSize);
}

void Game::mClearGame()
{
	if (mBoard != nullptr)
	{
		for (int x = 0; x < mBoardTilesNumber; x++)
		{
			delete[] mBoard[x];
		}
		delete[] mBoard;
		mBoard = nullptr;
	}

	mCordSnake.clear();
}

void Game::mDrawString(std::string str, sf::Color col, float y, float x)
{
	if (x == -1)
		x = mWindow.getSize().x / 2.0;
	if (y == -1)
		y = mWindow.getSize().y / 2.0;
	sf::Font font;
	font.loadFromFile("Fonts\\arial.ttf");
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(col);
	text.setPosition(x, y);
	text.setString(str);
	text.setOrigin(text.getLocalBounds().width / 2.0, text.getLocalBounds().height / 2.0);
	mWindow.draw(text);

}

void Game::mAudioPrepare()
{
	mSoundBuffer[Sound::APPLE].loadFromFile("Sounds\\Apple.wav");

	mSound[Sound::APPLE].setBuffer(mSoundBuffer[Sound::APPLE]);
	mMusic[Music::GAMEOVER].openFromFile("Sounds\\GameOver.wav");
}