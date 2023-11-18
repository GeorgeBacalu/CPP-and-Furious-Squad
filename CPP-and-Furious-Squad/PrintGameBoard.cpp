#include "PrintGameBoard.h"

PrintGameBoard::PrintGameBoard(uint16_t dimension): m_dimension{dimension}
{
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
			if (std::find(pillars.begin(), pillars.end(), Pillar{ {i,j}, Color::BLACK }) != pillars.end())
				std::cout << "B";
			else if (std::find(pillars.begin(), pillars.end(), Pillar{ {i,j}, Color::RED }) != pillars.end())
				std::cout << "R";
			else
				std::cout<<".";
		}
		std::cout<<"|.\n";
	}
	std::cout << "  " << std::setfill('-') << std::setw(m_dimension + 1) << "\n";
	std::cout << "  " << std::setfill('.') << std::setw(m_dimension + 1) << "\n";
	std::cout<<"Dimension: "<<m_dimension<<"\n";
}

void PrintGameBoard::setDimension(uint16_t dimension)
{
		m_dimension = dimension;
}
