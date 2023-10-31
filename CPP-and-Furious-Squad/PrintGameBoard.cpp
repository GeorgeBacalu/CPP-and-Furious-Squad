#include "PrintGameBoard.h"

PrintGameBoard::PrintGameBoard(uint16_t dimension): m_dimension{dimension}
{
}

bool findPillar(const std::vector<Pillar>& pillars, const std::pair<int, int>& position, Pillar& p)
{
	for (auto pillar : pillars)
	{
		if (pillar.GetPosition() == position)
		{
			p = pillar;
			return true;
		}
	}
	return false;
}

void PrintGameBoard::print(const std::vector<Pillar>& pillars)
{
	//set width of each cell to 2
	std::cout.width(2);

	Pillar p;

	system("CLS");

	std::cout << "  " << std::setfill('.') << std::setw(m_dimension + 1) << "\n";
	std::cout << "  " << std::setfill('-') << std::setw(m_dimension + 1) << "\n";
	for(int i=0;i<m_dimension;i++)
	{
		std::cout<<".|";
		for(int j=0;j<m_dimension;j++)
		{
			if(findPillar(pillars, std::make_pair(i,j),p))
			{
				if (p.GetColor() == Color::BLACK)
					std::cout << "B";
				else if (p.GetColor() == Color::RED)
					std::cout << "R";
			}
			else
				std::cout<<".";
		}
		std::cout<<"|.\n";
	}
	std::cout << "  " << std::setfill('-') << std::setw(m_dimension + 1) << "\n";
	std::cout << "  " << std::setfill('.') << std::setw(m_dimension + 1) << "\n";
	std::cout<<"Dimension: "<<m_dimension<<"\n";
}
