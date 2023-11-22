#pragma once

#include <iostream>
#include "Utils.h"

using Score = std::pair<uint16_t, uint16_t>;

class ScoreBoard
{
	Score m_score;

	ScoreBoard();
public:
	static ScoreBoard& GetInstance();
	ScoreBoard(const ScoreBoard& other) = delete;
	ScoreBoard& operator=(const ScoreBoard& other) = delete;
	ScoreBoard(ScoreBoard&& other) = delete;
	ScoreBoard& operator=(ScoreBoard&& other) = delete;
	~ScoreBoard() = default;
	friend std::istream& operator>>(std::istream& in, ScoreBoard& scoreBoard);
	friend std::ostream& operator<<(std::ostream& out, const ScoreBoard& scoreBoard);

	const Score& GetScore() const;
	void SetScore(const Score& score);
	void SetScore(uint16_t redScore, uint16_t blackScore);

	void UpdateScore(Color color);
	void ResetScore();
};