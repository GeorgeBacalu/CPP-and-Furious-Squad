#include "PrintGameBoard.h"
#include "GameBoard.h"
#include "Bridge.h"
#include <fstream>
#include <thread>
#include <chrono>

void readPillars(std::vector<Pillar>& pillars);
void readBridges(std::vector<Bridge>& bridges);
void displayPillars(const std::vector<Pillar>& pillars);
void displayBridges(const std::vector<Bridge>& bridges);

int main()
{
	std::ofstream fout{ "game-board.prodb" };
	std::vector<Pillar> pillars;
	std::vector<Bridge> bridges;
	pillars.reserve(pow(BOARD_SIZE / 2, 2));
	bridges.reserve((BOARD_SIZE / 2) * (BOARD_SIZE / 2 - 1));

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

	/*readPillars(pillars);
	readBridges(bridges);
	std::cout << "PILLARS:\n";
	displayPillars(pillars);
	std::cout << "BRIDGES:\n";
	displayBridges(bridges);
	system("pause");*/
	
	GameBoard* gb = GameBoard::getInstance();
	//gb->setBridges(bridges);
	std::vector<std::vector<std::optional<Pillar>>> matrix(BOARD_SIZE);
	for (int i = 0; i < BOARD_SIZE; ++i)
		matrix[i].resize(BOARD_SIZE);
		
	for (auto pi : pillars)
		matrix[pi.GetPosition().first][pi.GetPosition().second] = pi;
	gb->setMatrix(matrix);
	gb->ListaAdiacentaInit();
	for (auto it : gb->getListaAdiacenta())
	{
		for (auto it2 : it)
			std::cout << it2 << ' ';
		std::cout<<"\n";
	}
	gb->EndingPillarsInit();
	for (auto it : gb->getEndingPillars())
	{
		gb->bfs(it);
	}

	gb->ProcessNextMove({ 2, 2 }); // red
	gb->ProcessNextMove({ 21, 2 }); // black
	gb->ProcessNextMove({ 6, 2 }); // red
	gb->ProcessNextMove({ 17, 2 }); // black
	gb->ProcessNextMove({ 4, 3 }); // red
	gb->ProcessNextMove({ 19, 3 }); // black
	displayBridges(gb->getBridges());
	displayPillars(gb->getPillars());
	fout << *gb;
	return 0;
}

void readPillars(std::vector<Pillar>& pillars) {
	std::ifstream inPillar{ "pillars.prodb" };
	if (!inPillar) { std::cerr << "Failed to open pillars.prodb for reading!\n"; return; }
	Pillar pillar;
	while (true) {
		try {
			inPillar >> pillar;
			pillars.push_back(pillar);
		}
		catch (const std::invalid_argument& e) {
			std::cerr << e.what() << std::endl;
			break;
		}
	}
}

void readBridges(std::vector<Bridge>& bridges) {
	std::ifstream inBridge{ "bridges.prodb" };
	if (!inBridge) { std::cerr << "Failed to open bridges.prodb for reading!\n"; return; }
	Bridge bridge;
	while (true) {
		try {
			inBridge >> bridge;
			bridges.push_back(bridge);
		}
		catch (const std::invalid_argument& e) {
			std::cerr << e.what() << std::endl;
			break;
		}
	}
}

void displayPillars(const std::vector<Pillar>& pillars) {
	for (const auto& pillar : pillars)
	{
		std::cout << pillar << "\n";
	}
}

void displayBridges(const std::vector<Bridge>& bridges) {
	for (const auto& bridge : bridges)
	{
		std::cout << bridge << "\n";
	}
}