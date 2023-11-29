#include "GameBoard.h"

module RandareConsola;

import <iostream>;
import <iomanip>;

ConsoleRenderer::ConsoleRenderer()
{
}

void ConsoleRenderer::Render(GameBoard* gb)
{
	system("CLS");
	std::cout << "  ";
	for(int i=1;i<GameBoard::kWidth - 1;++i)
		if(gb->getMatrix()[0][i].has_value())
			if(static_cast<int>(gb->getMatrix()[0][i].value().GetColor())==0)
				std::cout << "R";
			else
				std::cout << "B";
		else
			std::cout << ".";
	std::cout << "\n  " << std::setfill('-') << std::setw(GameBoard::kWidth - 1) << "\n";
	for (int i = 1; i < GameBoard::kWidth - 1; ++i)
	{
		if(gb->getMatrix()[i][0].has_value())
			if(static_cast<int>(gb->getMatrix()[i][0].value().GetColor())==0)
				std::cout << "R|";
			else
				std::cout << "B|";
		else
			std::cout << ".|";
		for (int j = 1; j < GameBoard::kHeight - 1; ++j)
		{
			if(gb->getMatrix()[i][j].has_value())
				if(static_cast<int>(gb->getMatrix()[i][j].value().GetColor())==0)
					std::cout << "R";
				else
					std::cout << "B";
			else
				std::cout << ".";
		}
		if (gb->getMatrix()[i][GameBoard::kHeight - 1].has_value())
			if (static_cast<int>(gb->getMatrix()[i][GameBoard::kHeight - 1].value().GetColor()) == 0)
				std::cout << "|R\n";
			else
				std::cout << "|B\n";
		else
			std::cout << "|.\n";
	}
	std::cout << "  " << std::setfill('-') << std::setw(GameBoard::kWidth - 1) << "\n";
	std::cout << "  ";
	for (int i = 1; i < GameBoard::kWidth - 1; ++i)
		if (gb->getMatrix()[GameBoard::kWidth - 1][i].has_value())
			if (static_cast<int>(gb->getMatrix()[GameBoard::kWidth - 1][i].value().GetColor()) == 0)
				std::cout << "R";
			else
				std::cout << "B";
		else
			std::cout << ".";
	std::cout << "\nDimension: " << GameBoard::kWidth << "\n";
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
	for(auto & i:gb->getEndingPillars())
		gb->bfs(i);
}
