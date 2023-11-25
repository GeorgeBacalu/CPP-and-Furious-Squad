#include "ConsoleRenderer.h"

PrintGameBoard ConsoleRenderer::g{ 24 };

ConsoleRenderer::ConsoleRenderer()
{
}

void ConsoleRenderer::Render(GameBoard* gb)
{
	std::vector<Pillar> pillars;
	std::vector<Bridge> bridges;
	pillars.reserve((int)pow(BOARD_SIZE / 2, 2));
	bridges.reserve((BOARD_SIZE / 2) * (BOARD_SIZE / 2 - 1));

	for (auto linie : gb->getMatrix())
		for (auto coloana : linie)
			if (coloana.has_value())
				pillars.push_back(coloana.value());

	g.print(pillars);
}
