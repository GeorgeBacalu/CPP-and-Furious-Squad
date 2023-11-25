#pragma once

#include "PrintGameBoard.h"
#include "GameBoard.h"
#include "Bridge.h"
#include <vector>
#include <optional>

class TwixtGame {
	static void readPillars(std::vector<Pillar>& pillars);
	static void readBridges(std::vector<Bridge>& bridges);
	static void displayPillars(const std::vector<Pillar>& pillars);
	static void displayBridges(const std::vector<Bridge>& bridges);
public:
	static void Run();
};