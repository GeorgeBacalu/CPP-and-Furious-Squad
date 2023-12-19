#pragma once
#include "GameBoardCore.h"
#include "GameController.h"
#include <iostream>
#include <fstream>

class GameIOManager
{
	GameBoardCore& m_gameBoardCore;
	GameController& m_gameController;
public:
	GameIOManager(GameBoardCore& gameBoardCore, GameController& gameController);
	GameIOManager(const GameIOManager&) = delete;
	GameIOManager& operator=(const GameIOManager&) = delete;
	~GameIOManager() = default;

	// Logic methods
	void LoadGame();
	void LoadPillarsFromFile(const std::string& filename);
	void LoadBridgesFromFile(const std::string& filename);
	void SaveGame();
};