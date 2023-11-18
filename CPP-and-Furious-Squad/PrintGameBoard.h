#pragma once

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "Pillar.h"

class PrintGameBoard
{
	uint16_t m_dimension;
public:
	PrintGameBoard() = default;
	PrintGameBoard(uint16_t dimension);
	~PrintGameBoard() = default;
	void print(const std::vector<Pillar>& pillars);
	void setDimension(uint16_t dimension);

};

