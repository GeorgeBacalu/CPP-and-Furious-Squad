#pragma once

#include <iostream>

class ScoreBoard
{
	std::pair<uint16_t, uint16_t> score;
public:
	ScoreBoard();
	~ScoreBoard();
	ScoreBoard(const ScoreBoard& other);
	ScoreBoard& operator=(const ScoreBoard& other);

};

