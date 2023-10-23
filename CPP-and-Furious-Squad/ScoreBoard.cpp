#include "ScoreBoard.h"

ScoreBoard::ScoreBoard() : m_score{ 0, 0 }
{
}

ScoreBoard::~ScoreBoard()
{
}

ScoreBoard::ScoreBoard(const ScoreBoard& other) : m_score{ other.m_score }
{
}

ScoreBoard& ScoreBoard::operator=(const ScoreBoard& other)
{
	if (this != &other)
	{
		m_score = other.m_score;
	}
	return *this;
}

std::pair<uint16_t, uint16_t> ScoreBoard::GetScore() const
{
	return m_score;
}

void ScoreBoard::SetScore(std::pair<uint16_t, uint16_t> score)
{
	m_score = score;
}

void ScoreBoard::SetScore(uint16_t scoreRed, uint16_t scoreBlack)
{
	m_score.first = scoreRed;
	m_score.second = scoreBlack;
}
