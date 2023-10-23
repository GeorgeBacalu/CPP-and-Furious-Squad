#include "ScoreBoard.h"

ScoreBoard::ScoreBoard() : score{ 0, 0 }
{
}

ScoreBoard::~ScoreBoard()
{
}

ScoreBoard::ScoreBoard(const ScoreBoard& other) : score{ other.score }
{
}

ScoreBoard& ScoreBoard::operator=(const ScoreBoard& other)
{
	if (this != &other)
	{
		score = other.score;
	}
	return *this;
}
