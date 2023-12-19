#include "GameBoardCore.h"

GameBoardCore::GameBoardCore() : m_matrix{}
{
}

// Getters and Setters

uint16_t GameBoardCore::GetWidth() const
{
	return kWidth;
}

uint16_t GameBoardCore::GetHeight() const
{
	return kHeight;
}

std::array<std::array<std::optional<Pillar>, GameBoardCore::kWidth>, GameBoardCore::kHeight> GameBoardCore::GetMatrix() const
{
	return m_matrix;
}

const std::vector<std::vector<Pillar>>& GameBoardCore::GetAdjacencyList() const
{
	return m_adjacencyList;
}

const std::vector<std::vector<Pillar>>& GameBoardCore::GetRedPaths() const
{
	return m_redPaths;
}

const std::vector<std::vector<Pillar>>& GameBoardCore::GetBlackPaths() const
{
	return m_blackPaths;
}

std::vector<Bridge> GameBoardCore::GetBridges() const
{
	return m_bridges;
}

void GameBoardCore::SetMatrix(const std::array<std::array<std::optional<Pillar>, kWidth>, kHeight>& matrix)
{
	for (size_t row = 0; row < kHeight; ++row)
		std::ranges::copy(matrix[row], m_matrix[row].begin());
}

void GameBoardCore::SetAdjacencyList(const std::vector<std::vector<Pillar>>& adjacencyList)
{
	m_adjacencyList = adjacencyList;
}

void GameBoardCore::SetRedPaths(const std::vector<std::vector<Pillar>>& redPaths)
{
	m_redPaths = redPaths;
}

void GameBoardCore::SetBlackPaths(const std::vector<std::vector<Pillar>>& blackPaths)
{
	m_blackPaths = blackPaths;
}

void GameBoardCore::SetBridges(const std::vector<Bridge>& bridges)
{
	m_bridges = bridges;
}

// Overloaded operators

std::optional<Pillar>& GameBoardCore::operator[](const Position& position)
{
	const auto& [row, column] = position;
	if (row > kHeight || column > kWidth)
	{
		throw std::out_of_range("Position out of bounds");
	}
	return m_matrix[row][column];
}

const std::optional<Pillar>& GameBoardCore::operator[](const Position& position) const
{
	const auto& [row, column] = position;
	if (row > kHeight || column > kWidth)
	{
		throw std::out_of_range("Position out of bounds");
	}
	return m_matrix[row][column];
}

std::ostream& operator<<(std::ostream& out, const GameBoardCore& gameBoardCore)
{
	size_t width = gameBoardCore.kWidth;
	size_t height = gameBoardCore.kHeight;
	for (size_t row = 0; row < width; row++)
	{
		for (size_t column = 0; column < height; column++)
		{
			if (row == 0 && column == 0 || row == 0 && column == height - 1 || row == width - 1 && column == 0 || row == width - 1 && column == height - 1)
				out << "   ";
			else if (!gameBoardCore.m_matrix[row][column].has_value())
				out << 0 << "  ";
			else
				out << (gameBoardCore.m_matrix[row][column].value().GetColor() == Color::RED ? 1 : 2) << "  ";
		}
		out << "\n\n";
	}
	return out;
}

// Logic methods

bool GameBoardCore::IsFreeFoundation(uint16_t row, uint16_t column)
{
	if ((row == 0 && column == 0) || (row == 0 && column == kWidth - 1) || (row == kHeight - 1 && column == 0) || (row == kHeight - 1 && column == kWidth - 1))
		return false;
	return !m_matrix[row][column].has_value();
}

void GameBoardCore::InitAdjacencyList()
{
	for (const auto& bridge : m_bridges)
	{
		const auto& [startRow, startColumn] = bridge.GetStartPillar().GetPosition();
		const auto& [endRow, endColumn] = bridge.GetEndPillar().GetPosition();
		m_adjacencyList[startRow * kWidth + startColumn].push_back(bridge.GetEndPillar());
		m_adjacencyList[endRow * kWidth + endColumn].push_back(bridge.GetStartPillar());
	}
}

void GameBoardCore::UpdateAdjacencyList()
{
	if (nrBridges < m_bridges.size())
	{
		const auto& lastBridge = m_bridges.back();
		const auto& [startRow, startColumn] = lastBridge.GetStartPillar().GetPosition();
		const auto& [endRow, endColumn] = lastBridge.GetEndPillar().GetPosition();
		m_adjacencyList[startRow * kWidth + startColumn].push_back(lastBridge.GetEndPillar());
		m_adjacencyList[endRow * kHeight + endColumn].push_back(lastBridge.GetStartPillar());
	}
}

void GameBoardCore::BFS(const Pillar& start)
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
			const auto& [nodeRow, nodeColumn] = current.GetPosition();
			if (!visited[nodeRow * kWidth + nodeColumn] && node.GetColor() == start.GetColor())
				queue.push(node);
		}
	}
	start.GetColor() == Color::RED ? m_redPaths = paths : m_blackPaths = paths;
}