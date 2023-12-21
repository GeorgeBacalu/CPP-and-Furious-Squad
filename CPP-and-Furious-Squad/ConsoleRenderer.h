#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include "Pillar.h"
#include "Bridge.h"
#include "GameBoard.h"

class ConsoleRenderer
{
public:
	ConsoleRenderer();
	~ConsoleRenderer() = default;
	static void Render(GameBoard* gb);
	static void PrintCell(const std::optional<Pillar>& cell, bool withBorder = false, bool endOfLine = false);
	static void TakeInput(GameBoard* gb);
	static  std::vector<Bridge> PlaceBridgesFromOptions(const std::vector<Bridge>& bridgeOptions, uint16_t numToPlace);
	static  std::vector<Bridge> PlaceRandomBridgesFromOptions(const std::vector<Bridge>& bridgeOptions, uint16_t numToPlace);
};