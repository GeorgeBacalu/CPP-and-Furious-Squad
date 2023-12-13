#include "TwixtGame.h"
//#include "ConsoleRenderer.h"
#include <fstream>
#include <thread>
#include <chrono>
#include <iostream>
import RandareConsola;

void TwixtGame::Run()
{
	std::ofstream fout{ "game-board.prodb" };
	std::vector<Pillar> pillars;
	std::vector<Bridge> bridges;
	pillars.reserve(GameBoard::kWidth / 2 * GameBoard::kHeight / 2 + 1);
	bridges.reserve(GameBoard::kWidth / 2 * (GameBoard::kHeight / 2 - 1) + 1);

	/*Point p1{ 3, 2 };
	pillars.emplace_back(p1, Color::RED);
	PrintGameBoard g{ 24 };
	g.print(pillars);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	p1 = { 5, 3 };
	pillars.emplace_back(p1, Color::RED);
	g.print(pillars);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	p1 = { 12, 9 };
	pillars.emplace_back(p1, Color::BLACK);
	g.print(pillars);
	system("pause");*/

	readPillars(pillars);
	readBridges(bridges);
	std::cout << "PILLARS:\n";
	displayPillars(pillars);
	std::cout << "BRIDGES:\n";
	displayBridges(bridges);
	system("pause");

	GameBoard* gameBoard = GameBoard::GetInstance();
	std::vector<std::vector<std::optional<Pillar>>> matrix(GameBoard::kWidth);
	for (int i = 0; i < GameBoard::kWidth; ++i)
		matrix[i].resize(GameBoard::kWidth);

	for (auto pillar : pillars)
	{
		const auto& [row, column] = pillar.GetPosition();
		matrix[row][column] = pillar;
	}
	//gameBoard->setMatrix(matrix);

	while (true)
	{
		//add key listener
		ConsoleRenderer::Render(gameBoard);
		ConsoleRenderer::TakeInput(gameBoard);
		if (gameBoard->CheckWin(Color::BLACK))
		{
			ConsoleRenderer::Render(gameBoard);
			std::cout << "Black wins!\n";
			break;
		}
		if (gameBoard->CheckWin(Color::RED))
		{
			ConsoleRenderer::Render(gameBoard);
			std::cout << "Red wins!\n";
			break;
		}
	}

	//gameBoard->ListaAdiacentaInit();
	//for (auto it : gameBoard->getListaAdiacenta())
	//{
	//	for (auto it2 : it)
	//		std::cout << it2 << ' ';
	//	std::cout << "\n";
	//}
	//gameBoard->EndingPillarsInit();
	//for (auto it : gameBoard->getEndingPillars())
	//{
	//	gameBoard->bfs(it);
	//}

	//try
	//{
	//	gameBoard->PlacePillar(2, 2); // red
	//	gameBoard->PlacePillar(21, 2); // black
	//	gameBoard->PlacePillar(6, 2); // red
	//	gameBoard->PlacePillar(17, 2); // black
	//	gameBoard->PlacePillar(4, 3); // red
	//	gameBoard->PlacePillar(19, 3); // black
	//	displayBridges(gameBoard->getBridges());
	//	displayPillars(gameBoard->getPillars());
	//}
	//catch (const std::invalid_argument& e)
	//{
	//	std::cerr << e.what() << std::endl;
	//	gameBoard->setInvalid(true);
	//}

	//fout << *gameBoard;
}

void TwixtGame::readPillars(std::vector<Pillar>& pillars)
{
	std::ifstream inPillar{ "pillars.prodb" };
	if (!inPillar)
	{
		std::cerr << "Failed to open pillars.prodb for reading!\n"; return;
	}
	Pillar pillar;
	while (true)
	{
		try
		{
			inPillar >> pillar;
			pillars.push_back(pillar);
		}
		catch (const std::invalid_argument& exception)
		{
			std::cerr << exception.what() << std::endl;
			break;
		}
	}
}

void TwixtGame::readBridges(std::vector<Bridge>& bridges)
{
	std::ifstream inBridge{ "bridges.prodb" };
	if (!inBridge)
	{
		std::cerr << "Failed to open bridges.prodb for reading!\n"; return;
	}
	Bridge bridge;
	while (true)
	{
		try
		{
			inBridge >> bridge;
			bridges.push_back(bridge);
		}
		catch (const std::invalid_argument& exception)
		{
			std::cerr << exception.what() << std::endl;
			break;
		}
	}
}

void TwixtGame::displayPillars(const std::vector<Pillar>& pillars)
{
	for (const auto& pillar : pillars)
	{
		std::cout << pillar << "\n";
	}
}

void TwixtGame::displayBridges(const std::vector<Bridge>& bridges)
{
	for (const auto& bridge : bridges)
	{
		std::cout << bridge << "\n";
	}
}