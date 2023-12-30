#include "GameBoard.h"
#include "AiPlayer.h"

export module RandareConsola;


export class ConsoleRenderer
{
public:
	ConsoleRenderer();
	~ConsoleRenderer() = default;
	static void Render(GameBoard* gb);
	static void TakeInput(GameBoard* gb);
	static void TakeInputWithAi(GameBoard* gb, AiPlayer* ai);
	static void TakeInputWithAi2(GameBoard* gb, AiPlayer* ai, AiPlayer* ai2);
};