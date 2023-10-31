#pragma once

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "Pillar.h"

class PrintGameBoard
{
	uint16_t m_dimension;
public:
	PrintGameBoard(uint16_t dimension);
	void print(const std::vector<Pillar>& pillars);

};
