#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include "Pillar.h"
#include "Bridge.h"
#include "GameBoard.h"
#include "PrintGameBoard.h"

class ConsoleRenderer
{
	static PrintGameBoard g;
public:
	ConsoleRenderer();
	~ConsoleRenderer()=default;
	static void Render(std::vector<Pillar>pillars);
};

