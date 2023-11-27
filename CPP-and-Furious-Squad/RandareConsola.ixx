#include "GameBoard.h"

export module RandareConsola;


export class ConsoleRenderer
{
public:
	ConsoleRenderer();
	~ConsoleRenderer() = default;
	static void Render(GameBoard* gb);
	static void TakeInput(GameBoard* gb);
};