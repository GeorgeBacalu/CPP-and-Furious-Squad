#include "ConsoleRenderer.h"

ConsoleRenderer::ConsoleRenderer()
{
}

void ConsoleRenderer::Render(GameBoard* gb)
{
	system("CLS");
	std::cout << "  " << std::setfill('.') << std::setw(BOARD_SIZE + 1) << "\n";
	std::cout << "  " << std::setfill('-') << std::setw(BOARD_SIZE + 1) << "\n";
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		std::cout << ".|";
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			if(gb->getMatrix()[i][j].has_value())
				if(static_cast<int>(gb->getMatrix()[i][j].value().GetColor())==0)
					std::cout << "R";
				else
					std::cout << "B";
			else
				std::cout << ".";
		}
		std::cout << "|.\n";
	}
	std::cout << "  " << std::setfill('-') << std::setw(BOARD_SIZE + 1) << "\n";
	std::cout << "  " << std::setfill('.') << std::setw(BOARD_SIZE + 1) << "\n";
	std::cout << "Dimension: " << BOARD_SIZE << "\n";
}

void ConsoleRenderer::TakeInput(GameBoard* gb)
{
	std::cout<<"Enter the coordinates of the pillar you want to place: ";
	int x,y;
	std::cin>>x>>y;
	//place pillar and switch player
	gb->PlacePillar(x,y);
}
