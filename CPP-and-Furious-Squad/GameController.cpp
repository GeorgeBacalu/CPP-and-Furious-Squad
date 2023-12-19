#include "GameController.h"
#include "ConsoleRenderer.h"

static uint16_t kAvailableRedPillars{ 50 };
static uint16_t kAvailableBlackPillars{ 50 };
static uint16_t kAvailableRedBridges{ 50 };
static uint16_t kAvailableBlackBridges{ 50 };

GameController::GameController(GameBoardCore& gameBoardCore, IntersectionChecker& intersectionChecker) : m_gameBoardCore{ gameBoardCore }, m_intersectionChecker{ intersectionChecker }
{
}

// Getters and Setters

bool GameController::GetPlayerTurn() const
{
	return m_playerTurn;
}

bool GameController::GetInvalid() const
{
	return m_invalid;
}

std::vector<Pillar> GameController::GetRedPillars() const
{
	return m_redPillars;
}

std::vector<Pillar> GameController::GetBlackPillars() const
{
	return m_blackPillars;
}

std::vector<Pillar> GameController::GetEndPillars() const
{
	return m_endPillars;
}

uint16_t GameController::GetAvailablePieces(IPiece* pieceType, Color color)
{
	return (dynamic_cast<Pillar*>(pieceType))
		? (color == Color::RED ? kAvailableRedPillars : kAvailableBlackPillars)
		: (color == Color::RED ? kAvailableRedBridges : kAvailableBlackBridges);
}

void GameController::SetPlayerTurn(bool playerTurn)
{
	m_playerTurn = playerTurn;
}

void GameController::SetInvalid(bool invalid)
{
	m_invalid = invalid;
}

void GameController::SetRedPillars(const std::vector<Pillar>& redPillars)
{
	m_redPillars = redPillars;
}

void GameController::SetBlackPillars(const std::vector<Pillar>& blackPillars)
{
	m_blackPillars = blackPillars;
}

void GameController::SetEndPillars(const std::vector<Pillar>& endPillars)
{
	m_endPillars = endPillars;
}

// Logic methods

void GameController::SwitchPlayerTurn()
{
	m_playerTurn = !m_playerTurn;
}

void GameController::InitEndPillars()
{
	//generate endPillars from m_matrix
	for (uint16_t row = 0; row < GameBoardCore::kWidth; ++row)
		for (uint16_t column = 0; column < GameBoardCore::kHeight; ++column) {
			const auto& matrix = m_gameBoardCore.GetMatrix();
			if (matrix[row][column].has_value() && (row == 0 || row == GameBoardCore::kHeight - 1 || column == 0 || column == GameBoardCore::kWidth - 1))
				m_endPillars.push_back(matrix[row][column].value());
		}
}

bool GameController::CheckWin(Color playerColor)
{
	const auto& paths = playerColor == Color::RED ? m_gameBoardCore.GetRedPaths() : m_gameBoardCore.GetBlackPaths();
	for (const auto& path : paths)
	{
		const auto& [startRow, startColumn] = path.front().GetPosition();
		const auto& [endRow, endColumn] = path.back().GetPosition();
		if (playerColor == Color::RED && ((startRow == 0 && endRow == GameBoardCore::kHeight - 1) || (startRow == GameBoardCore::kHeight - 1 && endRow == 0)) ||
			(playerColor == Color::BLACK && ((startColumn == 0 && endColumn == GameBoardCore::kWidth - 1) || (startColumn == GameBoardCore::kWidth - 1 && endColumn == 0))))
			return true;
	}
	return false;
}

// Player move methods

void GameController::PlacePillar(uint16_t row, uint16_t column)
{
	if (m_gameBoardCore.IsFreeFoundation(row, column))
	{
		Pillar pillar{ {row, column}, m_playerTurn ? Color::RED : Color::BLACK };
		ProcessNextMove(pillar);
		if (m_invalid == false)
		{
			m_gameBoardCore.GetMatrix()[row][column] = std::optional<Pillar>{ pillar };
			pillar.GetColor() == Color::RED ? m_redPillars.push_back(pillar) : m_blackPillars.push_back(pillar);
		}
	}
	else
	{
		m_invalid = true;
		throw std::invalid_argument("Position is not valid");
	}
}

void GameController::ProcessNextMove(Pillar& newPillar)
{
	Color playerColor = newPillar.GetColor();
	ValidateNewPillarPlacement(newPillar, playerColor);
	std::vector<Bridge> newBridges = ProcessBridgesForNewPillar(newPillar);
	UpdateAvailablePieces(newBridges, newPillar);
	SwitchPlayerTurn();
}

void GameController::ValidateNewPillarPlacement(const Pillar& newPillar, Color playerColor)
{
	const auto& [newRow, newColumn] = newPillar.GetPosition();
	const std::vector<std::pair<int16_t, int16_t>> bridgeAllowedOffsets{ {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} };

	if ((newRow == 0 || newRow == GameBoardCore::kHeight - 1) && (newColumn == 0 || newColumn == GameBoardCore::kWidth - 1))
	{
		m_invalid = true;
		throw std::invalid_argument("Can't place pillar on any board corner!");
	}
	if ((playerColor == Color::RED && (newColumn == 0 || newColumn == GameBoardCore::kWidth - 1)) ||
		(playerColor == Color::BLACK && (newRow == 0 || newRow == GameBoardCore::kHeight - 1)))
	{
		m_invalid = true;
		throw std::invalid_argument(playerColor == Color::RED ? "Red player can't place pillar on first or last column!" : "Black player can't place pillar on first or last row!");
	}
}

const std::vector<Bridge>& GameController::ProcessBridgesForNewPillar(const Pillar& newPillar)
{
	const auto& [newRow, newColumn] = newPillar.GetPosition();
	std::vector<Pillar> playerPillars = (newPillar.GetColor() == Color::RED) ? m_redPillars : m_blackPillars;
	const std::vector<std::pair<int16_t, int16_t>> bridgeAllowedOffsets{ {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} };
	std::vector<Bridge> newBridges;

	for (auto& currentPillar : playerPillars)
	{
		const auto& [currentRow, currentColumn] = currentPillar.GetPosition();
		for (const auto& [offsetX, offsetY] : bridgeAllowedOffsets)
		{
			if (currentRow + offsetX == newRow && currentColumn + offsetY == newColumn)
			{
				if (m_intersectionChecker.CheckNoIntersections())
				{
					newBridges.emplace_back(currentPillar, newPillar);
					break;
				}
			}
		}
	}
	return newBridges;
}

void GameController::UpdateAvailablePieces(const std::vector<Bridge>& newBridges, const Pillar& newPillar)
{
	Color playerColor = newPillar.GetColor();

	if (newBridges.size() > 0)
	{
		auto& availableBridges = (playerColor == Color::RED) ? kAvailableRedBridges : kAvailableBlackBridges;
		if (availableBridges - newBridges.size() < 0)
		{
			std::vector<Bridge> chosenBridges = playerColor == Color::RED ? ConsoleRenderer::PlaceBridgesFromOptions(newBridges, availableBridges) : ConsoleRenderer::PlaceRandomBridgesFromOptions(newBridges, availableBridges);
			availableBridges = 0;
			auto bridges = m_gameBoardCore.GetBridges();
			bridges.insert(bridges.end(), chosenBridges.begin(), chosenBridges.end());
			for (const auto& bridge : chosenBridges)
				bridges.push_back(bridge);
		}
	}
	m_playerTurn ? --kAvailableRedPillars : --kAvailableBlackPillars;
}

void GameController::RemovePillar(uint16_t row, uint16_t column)
{
	if (!m_gameBoardCore.IsFreeFoundation(row, column))
	{
		m_gameBoardCore.GetMatrix()[row][column] = std::nullopt;
		auto bridges = m_gameBoardCore.GetBridges();
		for (auto it = bridges.begin(); it != bridges.end();)
		{
			if (it->GetEndPillar().GetPosition() == Position{ row,column } || it->GetStartPillar().GetPosition() == Position{ row,column })
				bridges.erase(it);
			else
				++it;
		}
	}
	else
	{
		m_invalid = true;
		throw std::invalid_argument("There is no pillar to erase");
	}
}

// Game flow methods

void GameController::ResetGame()
{
	/*std::cout << "Are you sure you want to reset the game?\n";
	std::cout << "1 for yes , 0 for no";
	bool ok;
	std::cin >> ok;*/
	for (uint16_t i = 0; i < GameBoardCore::kWidth; ++i)
		for (uint16_t j = 0; j < GameBoardCore::kHeight; ++j)
			m_gameBoardCore.GetMatrix()[i][j] = std::optional<Pillar>{};
	m_gameBoardCore.SetAdjacencyList(std::vector<std::vector<Pillar>>{ GameBoardCore::kWidth* GameBoardCore::kHeight });
	m_gameBoardCore.SetRedPaths({});
	m_gameBoardCore.SetBlackPaths({});
	m_gameBoardCore.SetBridges({});
	m_redPillars = {};
	m_blackPillars = {};
	m_endPillars = {};
}

// Related to AI player

int64_t GameController::GetHashWithPosition(const Position& position) const {
	std::hash<int64_t> hasher;
	int64_t hash = 0;
	const auto& [row, column] = position;
	auto matrix = m_gameBoardCore.GetMatrix();
	if (matrix[row][column].has_value())
	{
		Pillar pillar = matrix[row][column].value();
		hash = hasher(static_cast<int64_t>(pillar.GetColor()));
	}
	return hash;
}