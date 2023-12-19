#include "GameIOManager.h"

GameIOManager::GameIOManager(GameBoardCore& gameBoardCore, GameController& gameController) : m_gameBoardCore{ gameBoardCore }, m_gameController{ gameController }
{
}

// Logic methods

void GameIOManager::LoadGame()
{
	m_gameController.ResetGame();
	LoadPillarsFromFile("pillars.prodb");
	LoadBridgesFromFile("bridges.prodb");
}

void GameIOManager::LoadPillarsFromFile(const std::string& filename)
{
	std::ifstream fp{ filename };
	Pillar pillar;
	while (fp >> pillar)
	{
		try
		{
			const auto& [row, column] = pillar.GetPosition();
			m_gameController.PlacePillar(row, column);
		}
		catch (const std::invalid_argument& exception)
		{
			std::cerr << exception.what() << std::endl;
			break;
		}
	}
	fp.close();
}

void GameIOManager::LoadBridgesFromFile(const std::string& filename)
{
	std::ifstream fb{ filename };
	Bridge bridge;
	while (fb >> bridge)
	{
		try
		{
			m_gameBoardCore.GetBridges().push_back(bridge);
		}
		catch (const std::invalid_argument& exception)
		{
			std::cerr << exception.what() << std::endl;
			break;
		}
	}
	fb.close();
}

void GameIOManager::SaveGame()
{
	std::ofstream fp{ "pillars.prodb" };
	for (auto row : m_gameBoardCore.GetMatrix())
	{
		for (auto optionalPillar : row)
		{
			if (optionalPillar.has_value())
				fp << optionalPillar.value();
		}
	}
	fp.close();
	std::ofstream fb{ "bridges.prodb" };
	for (const auto& bridge : m_gameBoardCore.GetBridges())
		fb << bridge;
	fb.close();
}