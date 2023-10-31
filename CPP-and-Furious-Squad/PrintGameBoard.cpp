#include "PrintGameBoard.h"

PrintGameBoard::PrintGameBoard(uint16_t dimension): m_dimension{dimension}
{
}

void PrintGameBoard::printGameBoard()
{
	//set width of each cell to 2
	std::cout.width(2);
	for (int i = 0; i < m_dimension + 4; i++)
	{
		if (i == 0 or i == m_dimension + 3)
		{
			std::cout << "  " << std::setfill('.') << std::setw(m_dimension+1) << "\n";
		}
		else if(i==1 or i==m_dimension+2)
		{
			std::cout << "  " << std::setfill('-') << std::setw(m_dimension+1) << "\n";
		}
		else
		{
			std::cout<< ".|"<< std::setfill('.') << std::setw(m_dimension+3) << "|.\n";
		}
	}
}
