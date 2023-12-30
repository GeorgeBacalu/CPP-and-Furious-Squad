#include "GameBoard.h"
#include "ConsoleRenderer.h"

static GameBoard* instance = nullptr;
static uint16_t kAvailableRedPillars{ 50 };
static uint16_t kAvailableBlackPillars{ 50 };
static uint16_t kAvailableRedBridges{ 50 };
static uint16_t kAvailableBlackBridges{ 50 };

GameBoard::GameBoard() : m_matrix{}, m_adjacencyList{ kWidth * kHeight }, m_redPaths{}, m_blackPaths{}, m_redPillars{}, m_blackPillars{}, m_bridges{}, m_endPillars{}
{
}

GameBoard* GameBoard::GetInstance()
{
	if (instance == NULL)
		instance = new GameBoard();
	return instance;
}

// Getters and Setters

uint16_t GameBoard::GetWidth()
{
	return kWidth;
}

uint16_t GameBoard::GetHeight()
{
	return kHeight;
}

bool GameBoard::GetPlayerTurn()
{
	return m_playerTurn;
}

bool GameBoard::GetInvalid()
{
	return m_invalid;
}

const std::array<std::array<std::optional<Pillar>, GameBoard::kWidth>, GameBoard::kHeight>& GameBoard::GetMatrix()
{
	return m_matrix;
}

const std::vector<std::vector<Pillar>>& GameBoard::GetAdjacencyList()
{
	return m_adjacencyList;
}

const std::vector<std::vector<Pillar>>& GameBoard::GetRedPaths()
{
	return m_redPaths;
}

const std::vector<std::vector<Pillar>>& GameBoard::GetBlackPaths()
{
	return m_blackPaths;
}

const std::vector<Pillar>& GameBoard::GetRedPillars()
{
	return m_redPillars;
}

const std::vector<Pillar>& GameBoard::GetBlackPillars()
{
	return m_blackPillars;
}

const std::vector<Bridge>& GameBoard::GetBridges()
{
	return m_bridges;
}

const std::vector<Pillar>& GameBoard::GetEndPillars()
{
	return m_endPillars;
}

void GameBoard::SetPlayerTurn(bool playerTurn)
{
	m_playerTurn = playerTurn;
}

void GameBoard::SetInvalid(bool invalid)
{
	m_invalid = invalid;
}

void GameBoard::SetMatrix(const std::array<std::array<std::optional<Pillar>, kWidth>, kHeight>& matrix)
{
	for (size_t row = 0; row < kHeight; ++row)
		std::ranges::copy(matrix[row], m_matrix[row].begin());
}

void GameBoard::SetAdjacencyList(const std::vector<std::vector<Pillar>>& adjacencyList)
{
	m_adjacencyList = adjacencyList;
}

void GameBoard::SetRedPaths(const std::vector<std::vector<Pillar>>& redPaths)
{
	m_redPaths = redPaths;
}

void GameBoard::SetBlackPaths(const std::vector<std::vector<Pillar>>& blackPaths)
{
	m_blackPaths = blackPaths;
}

void GameBoard::SetRedPillars(const std::vector<Pillar>& redPillars)
{
	m_redPillars = redPillars;
}

void GameBoard::SetBlackPillars(const std::vector<Pillar>& blackPillars)
{
	m_blackPillars = blackPillars;
}

void GameBoard::SetBridges(const std::vector<Bridge>& bridges)
{
	m_bridges = bridges;
}

void GameBoard::SetEndPillars(const std::vector<Pillar>& endPillars)
{
	m_endPillars = endPillars;
}

uint16_t GameBoard::GetAvailablePieces(IPiece* pieceType, Color color)
{
	return (dynamic_cast<Pillar*>(pieceType))
		? (color == Color::RED ? kAvailableRedPillars : kAvailableBlackPillars)
		: (color == Color::RED ? kAvailableRedBridges : kAvailableBlackBridges);
}

// Overloaded operators

std::optional<Pillar>& GameBoard::operator[](const Position& position)
{
	const auto& [row, column] = position;
	if (row > kHeight || column > kWidth)
	{
		m_invalid = true;
		throw std::out_of_range("Position out of bounds");
	}
	return m_matrix[row][column];
}

const std::optional<Pillar>& GameBoard::operator[](const Position& position) const
{
	const auto& [row, column] = position;
	if (row > kHeight || column > kWidth)
	{
		throw std::out_of_range("Position out of bounds");
	}
	return m_matrix[row][column];
}

std::ostream& operator<<(std::ostream& out, const GameBoard& gameBoard)
{
	size_t width = gameBoard.kWidth;
	size_t height = gameBoard.kHeight;
	for (size_t row = 0; row < width; row++)
	{
		for (size_t column = 0; column < height; column++)
		{
			if (row == 0 && column == 0 || row == 0 && column == height - 1 || row == width - 1 && column == 0 || row == width - 1 && column == height - 1)
				out << "   ";
			else if (!gameBoard.m_matrix[row][column].has_value())
				out << 0 << "  ";
			else
				out << (gameBoard.m_matrix[row][column].value().GetColor() == Color::RED ? 1 : 2) << "  ";
		}
		out << "\n\n";
	}
	return out;
}

// Logic methods

void GameBoard::SwitchPlayerTurn()
{
	m_playerTurn = !m_playerTurn;
}

bool GameBoard::IsFreeFoundation(uint16_t row, uint16_t column)
{
	if ((row == 0 && column == 0) || (row == 0 && column == kWidth - 1) || (row == kHeight - 1 && column == 0) || (row == kHeight - 1 && column == kWidth - 1))
		return false;
	return !m_matrix[row][column].has_value();
}

void GameBoard::InitAdjacencyList()
{
	for (const auto& bridge : m_bridges)
	{
		++nrBridges;
		const auto& [startRow, startColumn] = bridge.GetStartPillar().GetPosition();
		const auto& [endRow, endColumn] = bridge.GetEndPillar().GetPosition();
		m_adjacencyList[startRow * kWidth + startColumn].push_back(bridge.GetEndPillar());
		m_adjacencyList[endRow * kWidth + endColumn].push_back(bridge.GetStartPillar());
	}
}

void GameBoard::UpdateAdjacencyList()
{
	if (nrBridges < m_bridges.size())
	{
		const auto& lastBridge = m_bridges.back();
		++nrBridges;
		const auto& [startRow, startColumn] = lastBridge.GetStartPillar().GetPosition();
		const auto& [endRow, endColumn] = lastBridge.GetEndPillar().GetPosition();
		m_adjacencyList[startRow * kWidth + startColumn].push_back(lastBridge.GetEndPillar());
		m_adjacencyList[endRow * kHeight + endColumn].push_back(lastBridge.GetStartPillar());
	}
}

void GameBoard::BFS(const Pillar& start)
{
	// Make paths from startPillar to endPillars. All the paths must have same color of pillars
	std::vector<std::vector<Pillar>> paths;
	std::vector<Pillar> path;
	std::vector<bool> visited(kWidth * kHeight, false);
	std::queue<Pillar> queue;

	UpdateAdjacencyList();
	queue.push(start);

	// The red_paths must have it's first element the start pillar
	while (!queue.empty())
	{
		Pillar current = queue.front();
		const auto& [row, column] = current.GetPosition();
		queue.pop();
		path.push_back(current);
		visited[row * kWidth + column] = true;
		if (current != start && (row == 0 || row == kWidth - 1))
		{
			paths.push_back(path);
			path.clear();
		}
		for (const auto& node : m_adjacencyList[row * kWidth + column])
		{
			const auto& [nodeRow, nodeColumn] = node.GetPosition();
			if (!visited[nodeRow * kWidth + nodeColumn] && node.GetColor() == start.GetColor())
				queue.push(node);
		}
	}
	start.GetColor() == Color::RED ? m_redPaths = paths : m_blackPaths = paths;
}

bool GameBoard::CheckWin(Color playerColor)
{
	const auto& paths = playerColor == Color::RED ? m_redPaths : m_blackPaths;
	for (const auto& path : paths)
	{
		const auto& [startRow, startColumn] = path.front().GetPosition();
		const auto& [endRow, endColumn] = path.back().GetPosition();
		if (playerColor == Color::RED && ((startRow == 0 && endRow == kHeight - 1) || (startRow == kHeight - 1 && endRow == 0)) ||
		   (playerColor == Color::BLACK && ((startColumn == 0 && endColumn == kWidth - 1) || (startColumn == kWidth - 1 && endColumn == 0))))
			return true;
	}
	return false;
}

void GameBoard::InitEndPillars()
{
	//generate endPillars from m_matrix
	for (uint16_t row = 0; row < kWidth; ++row)
		for (uint16_t column = 0; column < kHeight; ++column)
			if (m_matrix[row][column].has_value() && (row == 0 || row == kHeight - 1 || column == 0 || column == kWidth - 1))
				m_endPillars.push_back(m_matrix[row][column].value());
}

const bool GameBoard::IsPositionValid(const Position& position)
{
	const auto& [row, column] = position;
	if (row == 0 && column == 0 || row == 0 && column == kWidth - 1 || row == kHeight - 1 && column == 0 || row == kHeight - 1 && column == kWidth - 1)
		return false;
	return row < kHeight && column < kWidth;
}

// Player move methods

void GameBoard::PlacePillar(uint16_t row, uint16_t column)
{
	if (IsFreeFoundation(row, column))
	{
		Pillar pillar{ {row, column}, m_playerTurn ? Color::RED : Color::BLACK };
		ProcessNextMove(pillar);
		if (m_invalid == false)
		{
			m_matrix[row][column] = std::optional<Pillar>{ pillar };
			pillar.GetColor() == Color::RED ? m_redPillars.push_back(pillar) : m_blackPillars.push_back(pillar);
		}
	}
	else
	{
		m_invalid = true;
		throw std::invalid_argument("Position is not valid");
	}
}

void GameBoard::ProcessNextMove(Pillar& newPillar)
{
	Color playerColor = newPillar.GetColor();
	ValidateNewPillarPlacement(newPillar, playerColor);
	std::vector<Bridge> newBridges = ProcessBridgesForNewPillar(newPillar);
	UpdateAvailablePieces(newBridges, newPillar);
	SwitchPlayerTurn();
}

void GameBoard::ValidateNewPillarPlacement(const Pillar& newPillar, Color playerColor)
{
	const auto& [newRow, newColumn] = newPillar.GetPosition();
	const std::vector<std::pair<int16_t, int16_t>> bridgeAllowedOffsets{ {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} };

	if ((newRow == 0 || newRow == kHeight - 1) && (newColumn == 0 || newColumn == kWidth - 1))
	{
		m_invalid = true;
		throw std::invalid_argument("Can't place pillar on any board corner!");
	}
	if ((playerColor == Color::RED && (newColumn == 0 || newColumn == kWidth - 1)) ||
		(playerColor == Color::BLACK && (newRow == 0 || newRow == kHeight - 1)))
	{
		m_invalid = true;
		throw std::invalid_argument(playerColor == Color::RED ? "Red player can't place pillar on first or last column!" : "Black player can't place pillar on first or last row!");
	}
}

std::vector<Bridge> GameBoard::ProcessBridgesForNewPillar(const Pillar& newPillar)
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
				if (CheckNoIntersections())
				{
					newBridges.emplace_back(currentPillar, newPillar);
					break;
				}
			}
		}
	}
	return newBridges;
}

void GameBoard::UpdateAvailablePieces(const std::vector<Bridge>& newBridges, const Pillar& newPillar)
{
	Color playerColor = newPillar.GetColor();


	if (newBridges.size() > 0)
	{
		auto& availableBridges = (playerColor == Color::RED) ? kAvailableRedBridges : kAvailableBlackBridges;
		std::vector<Bridge> chosenBridges = playerColor == Color::RED ? ConsoleRenderer::PlaceRandomBridgesFromOptions(newBridges, newBridges.size()) : ConsoleRenderer::PlaceRandomBridgesFromOptions(newBridges, newBridges.size());
		availableBridges -=  chosenBridges.size();
		for (const auto& bridge : chosenBridges)
		{
			if (availableBridges > 0)
			{
				m_bridges.push_back(bridge);
				availableBridges--;
			}
			else
				break;//rest of the checks to be implemented later 
		}

	}
	m_playerTurn ? --kAvailableRedPillars : --kAvailableBlackPillars;
}
void GameBoard::PlacePillarQT(uint16_t row, uint16_t column)
{
	m_invalid = false;
	if (IsFreeFoundation(row, column))
	{
		Pillar pillar{ {row, column}, m_playerTurn ? Color::RED : Color::BLACK };
		ValidateNewPillarPlacement(pillar, pillar.GetColor());
		if (m_invalid == false)
		{
			m_matrix[row][column] = std::optional<Pillar>{ pillar };
			pillar.GetColor() == Color::RED ? m_redPillars.push_back(pillar) : m_blackPillars.push_back(pillar);
			SwitchPlayerTurn();
		}
	}
	else
	{
		m_invalid = true;
		throw std::invalid_argument("Position is not valid");
	}
}
void GameBoard::RemovePillar(uint16_t row, uint16_t column)
{
	if (!IsFreeFoundation(row, column))
	{
		m_matrix[row][column] = std::nullopt;
		for (auto it = m_bridges.begin(); it != m_bridges.end();)
		{
			if (it->GetEndPillar().GetPosition() == Position{ row,column } || it->GetStartPillar().GetPosition() == Position{ row,column })
				m_bridges.erase(it);
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

// Check intersection methods

bool GameBoard::CheckNoIntersections()
{
	if (m_bridges.empty())
		return true;
	Bridge newBridge = m_bridges.back();
	for (const auto& existingBridge : m_bridges)
		if (Intersects(existingBridge, newBridge))
			return false;
	return true;
}

bool GameBoard::Intersects(const Bridge& bridge1, const Bridge& bridge2)
{
	const auto& start1 = bridge1.GetStartPillar().GetPosition();
	const auto& end1 = bridge1.GetEndPillar().GetPosition();
	const auto& start2 = bridge2.GetStartPillar().GetPosition();
	const auto& end2 = bridge2.GetEndPillar().GetPosition();
	return (start1 == start2 || start1 == end2 || end1 == start2 || end1 == end2) && IntersectsOnSameAxis(bridge1, bridge2);
}

bool GameBoard::IntersectsOnSameAxis(const Bridge& bridge1, const Bridge& bridge2)
{
	const auto& [startRow1, startColumn1] = bridge1.GetStartPillar().GetPosition();
	const auto& [endRow1, endColumn1] = bridge1.GetEndPillar().GetPosition();
	const auto& [startRow2, startColumn2] = bridge2.GetStartPillar().GetPosition();
	const auto& [endRow2, endColumn2] = bridge2.GetEndPillar().GetPosition();

	if (startRow1 == endRow1)
		return startRow2 == endRow2 && startRow1 == startRow2 && IntersectsOnAxis(startColumn1, endColumn1, startColumn2, endColumn2);
	else if (startColumn1 == endColumn1)
		return startColumn2 == endColumn2 && startColumn1 == startColumn2 && IntersectsOnAxis(startRow1, endRow1, startRow2, endRow2);
	return false;
}

bool GameBoard::IntersectsOnAxis(size_t start1, size_t end1, size_t start2, size_t end2)
{
	return (start1 < end1) ? (start1 < start2 && start2 < end1) : (start1 > start2 && start2 > end1);
}

// Game flow methods

void GameBoard::LoadGame()
{
	ResetGame();
	LoadPillarsFromFile("pillars.prodb");
	LoadBridgesFromFile("bridges.prodb");
}
void GameBoard::PlacePillarFromFile(const Pillar& pillar)
{
	const auto& [row, column] = pillar.GetPosition();
	if (IsFreeFoundation(row, column))
	{
		m_matrix[row][column] = std::optional<Pillar>{ pillar };
		pillar.GetColor() == Color::RED ? m_redPillars.push_back(pillar) : m_blackPillars.push_back(pillar);
	}
	else
		throw std::invalid_argument("Position is not valid");
}


void GameBoard::LoadPillarsFromFile(const std::string& filename)
{
	std::ifstream fp{ filename };
	Pillar pillar;
	uint16_t blackCounter{ 0 };
	uint16_t redCounter{ 0 };
	while (!fp.eof())
	{
		try
		{
			fp >> pillar;
			PlacePillarFromFile(pillar);
			if (pillar.GetColor() == Color::RED)
				redCounter++;
			else
				blackCounter++;
		}
		catch (const std::invalid_argument& exception)
		{
			std::cerr << exception.what() << std::endl;
			break;
		}
	}
	if (redCounter > blackCounter)
		m_playerTurn = false;
	else
		m_playerTurn = true;
	fp.close();
}

void GameBoard::LoadBridgesFromFile(const std::string& filename)
{
	std::ifstream fb{ filename };
	Bridge bridge;
	while (!fb.eof())
	{
		try
		{
			fb >> bridge;
			m_bridges.push_back(bridge);
		}
		catch (const std::invalid_argument& exception)
		{
			std::cerr << exception.what() << std::endl;
			break;
		}
	}
	fb.close();
}

void GameBoard::SaveGame()
{
	std::ofstream fp{ "pillars.prodb" };
	for (auto row : m_matrix)
	{
		for (auto optionalPillar : row)
		{
			if (optionalPillar.has_value())
				fp << optionalPillar.value();
		}
	}
	fp.close();
	std::ofstream fb{ "bridges.prodb" };
	for (const auto& bridge : m_bridges)
		fb << bridge;
	fb.close();
}

void GameBoard::ResetGame()
{
	/*std::cout << "Are you sure you want to reset the game?\n";
	std::cout << "1 for yes , 0 for no";
	bool ok;
	std::cin >> ok;*/
	for (uint16_t i = 0; i < kWidth; ++i)
		for (uint16_t j = 0; j < kHeight; ++j)
			m_matrix[i][j] = std::optional<Pillar>{};
	m_adjacencyList = std::vector<std::vector<Pillar>>{ kWidth * kHeight };
	m_redPaths = {};
	m_blackPaths = {};
	m_redPillars = {};
	m_blackPillars = {};
	m_bridges = {};
	m_endPillars = {};
}

// Related to AI player

int64_t GameBoard::GetHashWithPosition(const Position& position) const {
	std::hash<int64_t> hasher;
	int64_t hash = 0;
	const auto& [row, column] = position;
	if (m_matrix[row][column].has_value())
	{
		Pillar pillar = m_matrix[row][column].value();
		hash = hasher(static_cast<int64_t>(pillar.GetColor()));
	}
	return hash;
}