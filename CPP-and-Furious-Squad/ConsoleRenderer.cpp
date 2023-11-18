#include "ConsoleRenderer.h"

PrintGameBoard ConsoleRenderer::g{ 24 };

ConsoleRenderer::ConsoleRenderer()
{
}

void ConsoleRenderer::Render(std::vector<Pillar> pillars)
{
	g.print(pillars);
}
