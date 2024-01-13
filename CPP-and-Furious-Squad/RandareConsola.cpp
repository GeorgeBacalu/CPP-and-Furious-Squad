#include "GameBoard.h"
#include <chrono>
#include <thread>

module RandareConsola;

import <iostream>;
import <iomanip>;

ConsoleRenderer::ConsoleRenderer()
{
}

void ConsoleRenderer::TakeInputWithAi(GameBoard* gb, AiPlayer* ai)
{
	int x, y;
	try
	{
		if (gb->GetPlayerTurn())
		{
			std::cout << "Red player's turn\n";
			std::cout << "Enter the coordinates of the pillar you want to place: ";
			std::cin >> x >> y;
		}
		else
		{
			std::cout << "AI player's turn\n";
			std::cout << "Enter the coordinates of the pillar you want to place: ";
			const auto& [row, column] = ai->GetNextAction();
			x = row;
			y = column;
			std::cout << x << " " << y << "\n";
			system("pause");
		}
	}
	catch (std::invalid_argument& exception)
	{
		std::cerr << exception.what() << "\n";
		throw (std::invalid_argument("TIE"));
	}
	//place pillar and switch player
	try
	{
		gb->PlacePillar(x, y);
		gb->InitEndPillars();
		std::cout << "OK\n";
		for (auto it : gb->GetEndPillars())
			gb->BFS(it);
	}
	catch (std::invalid_argument& exception)
	{
		std::cerr << exception.what() << "\n";
	}
	//ai->FreeReward(0.0f);
	//ai->FreeReward(0.0f);
	//ai->FreeReward	
}

void ConsoleRenderer::TakeInputWithAi2(GameBoard* gb, AiPlayer* ai, AiPlayer* ai2)
{
	int x, y;
	try
	{
		if (gb->GetPlayerTurn())
		{
			/*std::cout << "Red AI player's turn\n";
			std::cout << "Enter the coordinates of the pillar you want to place: ";*/
			const auto& [row, column] = ai->RandomAction();
			x = row;
			y = column;
			//std::cout << x << " " << y << "\n";
			//system("pause");
		}
		else
		{
			/*std::cout << "Black AI player's turn\n";
			std::cout << "Enter the coordinates of the pillar you want to place: ";*/
			const auto& [row, column] = ai2->GetNextAction();
			x = row;
			y = column;
			//std::cout << x << " " << y << "\n";
			//system("pause");
		}
	}
	catch (std::invalid_argument& exception)
	{
		//std::cerr << exception.what() << "\n";
		throw (std::invalid_argument("TIE"));
	}
	//place pillar and switch player
	try
	{
		gb->PlacePillar(x, y);
		gb->InitEndPillars();
		for (auto it : gb->GetEndPillars())
			gb->BFS(it);
	}
	catch (std::invalid_argument& exception)
	{
		std::cerr << exception.what() << "\n";
	}
	//ai->FreeReward(0.0f);
	//ai->FreeReward(0.0f);
	//ai->FreeReward
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
	for(const Bridge& bridge : gameBoard->GetBridges())
		std::cout << bridge << "\n";
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
		for (const auto& it : gameBoard->GetEndPillars())
		{
			gameBoard->UpdateAdjacencyList();
			if (it.GetColor() == Color::RED)
				gameBoard->SetRedPaths(GameBoard::BFS(it, gameBoard->GetAdjacencyList()));
			else
				gameBoard->SetBlackPaths(GameBoard::BFS(it, gameBoard->GetAdjacencyList()));
		}
	}
	catch (std::invalid_argument& exception)
	{
		std::cerr << exception.what() << "\n";
		throw (std::invalid_argument("TIE"));
		system("pause");
	}
}