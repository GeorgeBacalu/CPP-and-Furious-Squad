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
};