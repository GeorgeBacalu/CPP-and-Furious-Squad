#include "GameBoard.h"

GameBoard::GameBoard()
{
	s_size = uint16_t();
	s_matrix = std::vector<std::vector<std::shared_ptr<Pillar>>>();
}

GameBoard::GameBoard(const uint16_t& size, const std::vector<std::vector<std::shared_ptr<Pillar>>>& matrix)
{
	s_size = size;
	for (int i = 0; i < s_size; i++)
	{
		std::vector<std::shared_ptr<Pillar>> row;
		for (uint16_t j = 0; j < s_size; ++j) 
			row.push_back(std::make_shared<Pillar>(matrix[i][j]));
		s_matrix.push_back(row);
	}
}

GameBoard::GameBoard(const GameBoard& gb)
{
	s_size = gb.s_size;
	s_matrix = gb.s_matrix;
}

GameBoard& GameBoard::operator=(const GameBoard& gb)
{
	s_size = gb.s_size;
	s_matrix = gb.s_matrix;
}


