#include "ConsoleRenderer.h"
#include <iomanip>

ConsoleRenderer::ConsoleRenderer()
{
}

void ConsoleRenderer::Render(GameBoard* gb)
{
	system("CLS");
	std::cout << "  ";
	for(int i=1;i<BOARD_SIZE;++i)
		if(gb->getMatrix()[0][i].has_value())
			if(static_cast<int>(gb->getMatrix()[0][i].value().GetColor())==0)
				std::cout << "R";
			else
				std::cout << "B";
		else
			std::cout << ".";
	std::cout << "\n  " << std::setfill('-') << std::setw(BOARD_SIZE) << "\n";
	for (int i = 1; i < BOARD_SIZE; ++i)
	{
		if(gb->getMatrix()[i][0].has_value())
			if(static_cast<int>(gb->getMatrix()[i][0].value().GetColor())==0)
				std::cout << "R|";
			else
				std::cout << "B|";
		else
			std::cout << ".|";
		for (int j = 1; j < BOARD_SIZE; ++j)
		{
			if(gb->getMatrix()[i][j].has_value())
				if(static_cast<int>(gb->getMatrix()[i][j].value().GetColor())==0)
					std::cout << "R";
				else
					std::cout << "B";
			else
				std::cout << ".";
		}
		if (gb->getMatrix()[i][BOARD_SIZE - 1].has_value())
			if (static_cast<int>(gb->getMatrix()[i][BOARD_SIZE - 1].value().GetColor()) == 0)
				std::cout << "|R\n";
			else
				std::cout << "|B\n";
		else
			std::cout << "|.\n";
	}
	std::cout << "  " << std::setfill('-') << std::setw(BOARD_SIZE) << "\n";
	for (int i = 0; i < BOARD_SIZE; ++i)
		if (gb->getMatrix()[BOARD_SIZE - 1][i].has_value())
			if (static_cast<int>(gb->getMatrix()[BOARD_SIZE - 1][i].value().GetColor()) == 0)
				std::cout << "R";
			else
				std::cout << "B";
		else
			std::cout << ".";
	std::cout << "\nDimension: " << BOARD_SIZE << "\n";
}

void ConsoleRenderer::TakeInput(GameBoard* gb)
{
	if(gb->PlayerTurn())
		std::cout<<"Red player's turn\n";
	else
		std::cout<<"Black player's turn\n";
	std::cout<<"Enter the coordinates of the pillar you want to place: ";
	int x,y;
	std::cin>>x>>y;
	//place pillar and switch player
	gb->PlacePillar(x,y);
	gb->EndingPillarsInit();
	for (auto it : gb->getEndingPillars())
		gb->bfs(it);
}
