#pragma once
#include "Bridge.h"
#include <array>
#include <queue>
#include <optional>

class GameBoardCore
{
public:
	static constexpr size_t kWidth{ 5 };
	static constexpr size_t kHeight{ 5 };
private:
	uint16_t nrBridges{ 0 };
	std::array<std::array<std::optional<Pillar>, kWidth>, kHeight> m_matrix;
	std::vector<std::vector<Pillar>> m_adjacencyList;
	std::vector<std::vector<Pillar>> m_redPaths;
	std::vector<std::vector<Pillar>> m_blackPaths;
	std::vector<Bridge> m_bridges;
public:
	GameBoardCore();
	GameBoardCore(const GameBoardCore& other) = delete;
	GameBoardCore(GameBoardCore&& other) = delete;
	~GameBoardCore() = default;

	// Getters and Setters
	uint16_t GetWidth() const;
	uint16_t GetHeight() const;
	std::array<std::array<std::optional<Pillar>, kWidth>, kHeight> GetMatrix() const;
	const std::vector<std::vector<Pillar>>& GetAdjacencyList() const;
	const std::vector<std::vector<Pillar>>& GetRedPaths() const;
	const std::vector<std::vector<Pillar>>& GetBlackPaths() const;
	std::vector<Bridge> GetBridges() const;
	void SetMatrix(const std::array<std::array<std::optional<Pillar>, kWidth>, kHeight>& matrix);
	void SetAdjacencyList(const std::vector<std::vector<Pillar>>& adjacencyList);
	void SetRedPaths(const std::vector<std::vector<Pillar>>& redPaths);
	void SetBlackPaths(const std::vector<std::vector<Pillar>>& blackPaths);
	void SetBridges(const std::vector<Bridge>& bridges);

	// Overloaded operators
	std::optional<Pillar>& operator[](const Position& position);
	const std::optional<Pillar>& operator[](const Position& position) const;
	friend std::ostream& operator<<(std::ostream& out, const GameBoardCore& gameBoardCore);

	// Logic methods
	bool IsFreeFoundation(uint16_t row, uint16_t column);
	void InitAdjacencyList();
	void UpdateAdjacencyList();
	void BFS(const Pillar& start);
};