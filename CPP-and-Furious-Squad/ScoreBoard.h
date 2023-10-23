#pragma once

#include <iostream>

class ScoreBoard
{
	std::pair<uint16_t, uint16_t> m_score;
public:
	ScoreBoard();
	~ScoreBoard();
	ScoreBoard(const ScoreBoard& other);
	ScoreBoard& operator=(const ScoreBoard& other);
	std::pair<uint16_t, uint16_t> GetScore() const;
	void SetScore(std::pair<uint16_t, uint16_t> score);
	void SetScore(uint16_t scoreRed, uint16_t scoreBlack);
};

