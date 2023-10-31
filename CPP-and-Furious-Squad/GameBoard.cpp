#include "GameBoard.h"
uint16_t GameBoard::s_size = 0;
std::vector<std::vector<std::optional<Pillar>>>GameBoard::s_matrix;
GameBoard* GameBoard::instance = NULL;
GameBoard::GameBoard()
{
	GameBoard::s_size = 24;
	for (uint16_t i = 0; i < s_size; ++i)
	{
		std::vector<std::optional<Pillar>> row;
		for (uint16_t j = 0; j < s_size; ++j)
		{
			row.push_back(std::optional<Pillar>{});
		}
		GameBoard::s_matrix.push_back(row);
	}
}
GameBoard* GameBoard::getInstance()
{
	if (instance == NULL)
	{
		instance = new GameBoard();
		return instance;
	}
	else
		return instance;
}
uint16_t GameBoard::getSize()
{
	return s_size;
}

std::vector<std::vector<std::optional<Pillar>>> GameBoard::getMatrix()
{
	return s_matrix;
}

void GameBoard::setSize(uint16_t size)
{
	s_size = size;
}

void GameBoard::setMatrix(std::vector<std::vector<Pillar*>> matrix )
{
	s_matrix = std::vector<std::vector<Pillar*>>();
	for (uint16_t i = 0; i < s_size; ++i)
	{
		std::vector<Pillar*> row;
		for (uint16_t j = 0; j < s_size; ++j)
		{
			Pillar* aux = new Pillar(*matrix[i][j]);
			row.push_back(aux);
		}
		s_matrix.push_back(row);
	}
}

void GameBoard::PlacePillar(uint16_t row, uint16_t column)
{
	if (IsFreeFoundation(row, column))
	{
		s_matrix[row][column] = new Pillar();
		//the changes to color and rules for bridges are to be implemented later 
	}
}


void GameBoard::RemovePillar(uint16_t row, uint16_t column)
{
	s_matrix[row][column] = nullptr;
}

bool GameBoard::IsFreeFoundation(uint16_t row, uint16_t column)
{
	if (s_matrix[row][column] == nullptr)
		return true;
	return false;
}

void GameBoard::ResetGame()
{
	std::cout << "Are you sure you want to reset the game?\n";
	std::cout << "1 for yes , 0 for no";
	bool ok;
	std::cin >> ok;
	if (ok == 1)
	{
		for (uint16_t i = 0; i < s_size; ++i)
		{
			for (uint16_t j = 0; j < s_size; ++j)
				s_matrix[i][j] = nullptr;
		}
	}
}

std::ostream& operator<<(std::ostream& out, const GameBoard& gb)
{
	for (uint16_t i = 0; i < gb.s_size; i++)
	{
		for (uint16_t j = 0; j < gb.s_size; j++)
			out << gb.s_matrix[i][j];
	}
	return out;
}

