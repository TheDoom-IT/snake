#ifndef MYENUM_H
#define MYENUM_H

//Assigned values correspond with SpriteType values
//It allows to cast Difficulty and BoardSize to its SpriteType
enum class Difficulty
{
	EASY = 5, NORMAL = 6, HARD = 7
};

enum class BoardSize
{
	SMALL = 8, MEDIUM = 9, LARGE = 10
};

#endif