#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(std::string_view name) : m_name{ name }
{
}

Position HumanPlayer::GetNextAction()
{
	size_t row, column;
	std::cout << m_name << ", where do you want to put your pillar (row and column)?\n";
	std::cin >> row >> column;
	if(row >= Pillar::kWidth && column >= Pillar::kHeight)
		throw std::invalid_argument("Invalid position selected!");
	return { row, column };
}

std::string_view HumanPlayer::GetName() const
{
	return m_name;
}