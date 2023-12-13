#include "GameBoard.h"

module RandareConsola;

import <iostream>;
import <iomanip>;

ConsoleRenderer::ConsoleRenderer()
{
}

void ConsoleRenderer::Render(GameBoard* gameBoard)
{
	system("CLS");
	std::cout << "   ";
	for (int i = 1; i < GameBoard::kWidth - 1; ++i)
		std::cout << i;
	std::cout << "\n   ";
	for (int i = 1; i < GameBoard::kWidth - 1; ++i)
	{
		if (gameBoard->GetMatrix()[0][i].has_value())
			if (static_cast<int>(gameBoard->GetMatrix()[0][i].value().GetColor()) == 0)
				std::cout << "R";
			else
				std::cout << "B";
		else
			std::cout << ".";
	}
	std::cout << "\n   " << std::setfill('-') << std::setw(GameBoard::kWidth - 1) << "\n";
	for (int i = 1; i < GameBoard::kWidth - 1; ++i)
	{
		if (gameBoard->GetMatrix()[i][0].has_value())
			if (static_cast<int>(gameBoard->GetMatrix()[i][0].value().GetColor()) == 0)
				std::cout << i << "R|";
			else
				std::cout << i << "B|";
		else
			std::cout << i << ".|";
		for (int j = 1; j < GameBoard::kHeight - 1; ++j)
		{
			if (gameBoard->GetMatrix()[i][j].has_value())
				if (static_cast<int>(gameBoard->GetMatrix()[i][j].value().GetColor()) == 0)
					std::cout << "R";
				else
					std::cout << "B";
			else
				std::cout << ".";
		}
		if (gameBoard->GetMatrix()[i][GameBoard::kHeight - 1].has_value())
			if (static_cast<int>(gameBoard->GetMatrix()[i][GameBoard::kHeight - 1].value().GetColor()) == 0)
				std::cout << "|R\n";
			else
				std::cout << "|B\n";
		else
			std::cout << "|.\n";
	}
	std::cout << "   " << std::setfill('-') << std::setw(GameBoard::kWidth - 1) << "\n";
	std::cout << "   ";
	for (int i = 1; i < GameBoard::kWidth - 1; ++i)
		if (gameBoard->GetMatrix()[GameBoard::kWidth - 1][i].has_value())
			if (static_cast<int>(gameBoard->GetMatrix()[GameBoard::kWidth - 1][i].value().GetColor()) == 0)
				std::cout << "R";
			else
				std::cout << "B";
		else
			std::cout << ".";
	std::cout << "\nDimension: " << GameBoard::kWidth << "\n";
}

void ConsoleRenderer::TakeInput(GameBoard* gameBoard)
{
	if (gameBoard->GetPlayerTurn())
		std::cout << "Red player's turn\n";
	else
		std::cout << "Black player's turn\n";
	std::cout << "Enter the coordinates of the pillar you want to place: ";
	int x, y;
	std::cin >> x >> y;
	//place pillar and switch player
	try
	{
		gameBoard->PlacePillar(x, y);
		gameBoard->InitEndPillars();
		for (auto it : gameBoard->GetEndPillars())
			gameBoard->BFS(it);
	}
	catch (std::invalid_argument& exception)
	{
		std::cerr << exception.what() << "\n";
	}
}