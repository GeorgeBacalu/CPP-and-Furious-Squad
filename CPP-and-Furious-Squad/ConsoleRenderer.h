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
public:
	ConsoleRenderer();
	~ConsoleRenderer()=default;
	static void Render(GameBoard* gb);
};

