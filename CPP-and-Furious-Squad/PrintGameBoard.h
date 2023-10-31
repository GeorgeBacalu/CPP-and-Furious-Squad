#pragma once

#include <iostream>
#include "GameBoard.h"

class PrintGameBoard
{
	uint16_t m_dimension;
public:
	PrintGameBoard(uint16_t dimension);
	void printGameBoard();

};

