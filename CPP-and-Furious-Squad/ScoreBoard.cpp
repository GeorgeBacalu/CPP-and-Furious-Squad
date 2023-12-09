#include "ScoreBoard.h"

ScoreBoard::ScoreBoard() : m_score{ 0,0 }
{
}

ScoreBoard& ScoreBoard::GetInstance() 
{
	static ScoreBoard instance;
	return instance;
}

std::istream& operator>>(std::istream& in, ScoreBoard& scoreBoard)
{
	if (!(in >> scoreBoard.m_score.first >> scoreBoard.m_score.second))
		throw std::invalid_argument("Error reading scoreBoard input!");
	const auto& [redScore, blackScore] = scoreBoard.m_score;
	if (redScore >= ScoreBoard::kMaxScore || blackScore >= ScoreBoard::kMaxScore)
		throw std::invalid_argument("Invalid player(s) score!");
	return in;
}

std::ostream& operator<<(std::ostream& out, const ScoreBoard& scoreBoard)
{
	const auto& [redScore, blackScore] = scoreBoard.m_score;
	return out << "Red   " << redScore << " : " << blackScore << "   Black\n";
}

const Score& ScoreBoard::GetScore() const
{
	return m_score;
}

void ScoreBoard::SetScore(const Score& score) 
{
	const auto& [redScore, blackScore] = score;
	if (redScore >= kMaxScore || blackScore >= kMaxScore)
		throw std::out_of_range("Invalid player(s) score!");
	m_score = score;
}

void ScoreBoard::SetScore(uint16_t redScore, uint16_t blackScore)
{
	SetScore({ redScore, blackScore });
}

void ScoreBoard::UpdateScore(Color color)
{
	auto& [redScore, blackScore] = m_score;
	color == Color::RED ? ++redScore : ++blackScore;
}

void ScoreBoard::ResetScore()
{
	auto& [redScore, blackScore] = m_score;
	redScore = 0;
	blackScore = 0;
}