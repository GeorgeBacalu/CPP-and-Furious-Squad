#pragma once
#include "Bridge.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <queue>
#include <optional>

class GameBoard
{
public:
	static constexpr size_t kWidth{ 24 };
	static constexpr size_t kHeight{ 24 };
private:
	bool m_playerTurn{ true };
	bool m_invalid{ false };
	bool redPillarsSpent;
	bool redBridgesSpent;
	bool blackPillarsSpent;
	bool blackBridgesSpent;
	uint16_t nrBridges{ 0 };
	uint16_t redBridges;
	uint16_t blackBridges;
	std::array<std::array<std::optional<Pillar>, kWidth>, kHeight> m_matrix;
	std::vector<std::vector<Pillar>> m_adjacencyList;
	std::vector<std::vector<Pillar>> m_redPaths;
	std::vector<std::vector<Pillar>> m_blackPaths;
	std::vector<Pillar> m_redPillars;
	std::vector<Pillar> m_blackPillars;
	std::vector<Bridge> m_bridges;
	std::vector<Pillar> m_endPillars;

	void PlacePillarFromFile(const Pillar& pillar);
	GameBoard();
public:
	static GameBoard* GetInstance();

	GameBoard(const GameBoard& other) = delete;
	GameBoard& operator=(const GameBoard& other) = delete;
	~GameBoard() = default;

	// Getters and Setters
	uint16_t GetWidth();
	uint16_t GetHeight();
	bool GetPlayerTurn();
	bool GetInvalid();
	const std::array<std::array<std::optional<Pillar>, kWidth>, kHeight>& GetMatrix();
	const std::vector<std::vector<Pillar>>& GetAdjacencyList();
	const std::vector<std::vector<Pillar>>& GetRedPaths();
	const std::vector<std::vector<Pillar>>& GetBlackPaths();
	const std::vector<Pillar>& GetRedPillars();
	const std::vector<Pillar>& GetBlackPillars();
	const std::vector<Bridge>& GetBridges();
	const std::vector<Pillar>& GetEndPillars();
	bool getRedPillarsSpent() const;
	bool getRedBridgesSpent() const;
	bool getBlackPillarsSpent() const;
	bool getBlackBridgesSpent() const;
	uint16_t getRedBridgesCount();
	uint16_t getBlackBridgesCount();
	void SetPlayerTurn(bool playerTurn);
	void SetInvalid(bool invalid);
	void SetMatrix(const std::array<std::array<std::optional<Pillar>, kWidth>, kHeight>& matrix);
	void SetAdjacencyList(const std::vector<std::vector<Pillar>>& adjacencyList);
	void SetRedPaths(const std::vector<std::vector<Pillar>>& redPaths);
	void SetBlackPaths(const std::vector<std::vector<Pillar>>& blackPaths);
	void SetRedPillars(const std::vector<Pillar>& redPillars);
	void SetBlackPillars(const std::vector<Pillar>& blackPillars);
	void SetBridges(const std::vector<Bridge>& bridges);
	void SetEndPillars(const std::vector<Pillar>& endPillars);
	void setRedBridgesCount(uint16_t count);
	void setBlackBridgesCount(uint16_t count);

	// Overloaded operators
	std::optional<Pillar>& operator[](const Position& position);
	const std::optional<Pillar>& operator[](const Position& position) const;
	friend std::ostream& operator<<(std::ostream& out, const GameBoard& gameBoard);

	// Logic methods
	void SwitchPlayerTurn();
	bool IsFreeFoundation(uint16_t row, uint16_t column);
	void InitAdjacencyList();
	void UpdateAdjacencyList();
	void BFS(const Pillar& start);
	bool CheckWin(Color playerColor);
	void InitEndPillars();
	void checkPieces(bool playerTurn);

	const bool IsPositionValid(const Position& position);

	// Player move methods
	void PlacePillar(uint16_t row, uint16_t column);
	void AddBridge(const bool& playerTurn, const Bridge& bridge);
	void ProcessNextMove(Pillar& newPillar);
	void ValidateNewPillarPlacement(const Pillar& newPillar, Color playerColor);
	std::vector<Bridge> ProcessBridgesForNewPillar(const Pillar& newPillar);
	void UpdateAvailablePieces(const std::vector<Bridge>& newBridges, const Pillar& newPillar);
	void RemovePillar(uint16_t row, uint16_t column);
	void RemoveBridge(const Bridge& bridge);
	//QT player move methods
	void PlacePillarQT(uint16_t row, uint16_t column);

	// Check intersection methods - new
	/*bool CheckNoIntersections(const Bridge& newBridge);
	bool DoBridgesIntersect(const Bridge& bridge1, const Bridge& bridge2);
	bool INTERS(const Pillar& pillar1, const Pillar& pillar2, const Pillar& pillar3);*/

	// Check intersection methods - old
	bool CheckNoIntersections(const Bridge& newBridge);
	bool Intersects(const Bridge& bridge1, const Bridge& bridge2);
	bool INTERS(const Pillar& pillar1, const Pillar& pillar2, const Pillar& pillar3);
	bool INTERS_EQ(const Pillar& pillar1, const Pillar& pillar2, const Pillar& pillar3);
	bool IntersectsOnSameAxis(const Bridge& bridge1, const Bridge& bridge2);
	bool IntersectsOnAxis(size_t start1, size_t end1, size_t start2, size_t end2);

	// Game flow methods
	void LoadGame();
	void LoadPillarsFromFile(const std::string& filename);
	void LoadBridgesFromFile(const std::string& filename);
	void SaveGame();
	void ResetGame();

	// Related to AI player
	int64_t GetHashWithPosition(const Position& position) const;

	template<typename T>
	static std::vector<std::vector<T>> BFS(const T& start, const std::vector<std::vector<T>>& adjacencyList)
	{
		std::vector<std::vector<T>> paths;
		std::queue<T> queue;
		std::vector<T> path;
		std::unordered_map<T, bool> visited;
		std::unordered_map<T, T> parent;

		for (const auto& row : adjacencyList)
			for (const auto& node : row)
			{
				visited[node] = false;
				parent[node] = node;
			}



		queue.push({ start });
		visited[start] = true;

		while (!queue.empty())
		{
			T current = queue.front();
			queue.pop();
			for (const auto& node : adjacencyList[current])
			{
				if (!visited[node])
				{
					visited[node] = true;
					parent[node] = current;
					queue.push(node);
				}
			}
		}

		for (const auto& node : parent)
		{
			if (node.first != node.second)
			{
				path.push_back(node.first);
				if (node.first == start)
				{
					paths.push_back(path);
					path.clear();
				}
			}
		}
		return paths;
	}
	template<>
	static std::vector<std::vector<Pillar>> BFS(const Pillar& start, const std::vector<std::vector<Pillar>>& adjacencyList)
	{
		// Make paths from startPillar to endPillars. All the paths must have same color of pillars
		std::vector<std::vector<Pillar>> paths;
		std::vector<Pillar> path;
		std::vector<bool> visited(kWidth * kHeight, false);
		std::queue<Pillar> queue;

		//UpdateAdjacencyList();
		queue.push(start);

		// The red_paths must have it's first element the start pillar
		while (!queue.empty())
		{
			Pillar current = queue.front();
			const auto& [row, column] = current.GetPosition();
			queue.pop();
			path.push_back(current);
			visited[row * kWidth + column] = true;
			if (start.GetColor() == Color::RED)
			{
				if (current != start && (row == 0 || row == kWidth - 1))
				{
					paths.push_back(path);
					path.clear();
				}
			}
			else
			{
				if (current != start && (column == 0 || column == kWidth - 1))
				{
					paths.push_back(path);
					path.clear();
				}
			}
			for (const auto& node : adjacencyList[row * kWidth + column])
			{
				const auto& [nodeRow, nodeColumn] = node.GetPosition();
				if (!visited[nodeRow * kWidth + nodeColumn] && node.GetColor() == start.GetColor())
					queue.push(node);
			}
		}
		return paths;
	}
};