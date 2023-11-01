#include "GameBoard.h"
#include "ScoreBoard.h"
#include "Utils.h"
#include "Bridge.h"
#include "Pillar.h"
#include <fstream>

void readPillars(std::vector<Pillar>& pillars);
void readBridges(std::vector<Bridge>& bridges);
void displayPillars(const std::vector<Pillar>& pillars);
void displayBridges(const std::vector<Bridge>& bridges);

int main() 
{
	std::vector<Pillar> pillars;
	std::vector<Bridge> bridges;
	pillars.reserve(pow(BOARD_SIZE / 2, 2));
	bridges.reserve((BOARD_SIZE / 2) * (BOARD_SIZE / 2 - 1));
	readPillars(pillars);
	readBridges(bridges);
	std::cout << "PILLARS:\n";
	displayPillars(pillars);
	std::cout << "BRIDGES:\n";
	displayBridges(bridges);
}

void readPillars(std::vector<Pillar>& pillars) 
{
	std::ifstream inPillar{ "pillars.prodb" };
	if (!inPillar) { std::cerr << "Failed to open pillars.prodb for reading!\n"; return; }
	Pillar pillar;
	while (true) {
		try {
			if (!(inPillar >> pillar)) break;
			pillars.push_back(pillar);
		}
		catch (const std::invalid_argument& e) {
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
			if (!(inBridge >> bridge)) break;
			bridges.push_back(bridge);
		}
		catch (const std::invalid_argument& e) {
			break;
		}
	}
}

void displayPillars(const std::vector<Pillar>& pillars) {
	for (const auto& pillar : pillars) std::cout << pillar << "\n";
}

void displayBridges(const std::vector<Bridge>& bridges) {
	for (const auto& bridge : bridges) std::cout << bridge << "\n";
}