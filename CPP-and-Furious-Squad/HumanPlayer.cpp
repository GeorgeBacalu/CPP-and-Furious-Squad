#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(std::string_view name) : m_name{ name }
{
}

const Position& HumanPlayer::GetNextAction()
{
	int row, column;
	std::cout << m_name << ", where do you want to put your pillar (row and column)?\n";
	std::cin >> row >> column;
	return { row, column };
}

std::string_view HumanPlayer::GetName() const
{
	return m_name;
}