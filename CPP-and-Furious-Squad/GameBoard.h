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
	static constexpr size_t kWidth{ 5 };
	static constexpr size_t kHeight{ 5 };
private:
	bool m_playerTurn{ true };
	bool m_invalid{ false };
	uint16_t nrBridges{ 0 };
	std::array<std::array<std::optional<Pillar>, kWidth>, kHeight> m_matrix;
	std::vector<std::vector<Pillar>> m_adjacencyList;
	std::vector<std::vector<Pillar>> m_redPaths;
	std::vector<std::vector<Pillar>> m_blackPaths;
	std::vector<Pillar> m_redPillars;
	std::vector<Pillar> m_blackPillars;
	std::vector<Bridge> m_bridges;
	std::vector<Pillar> m_endPillars;

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
	uint16_t GetAvailablePieces(IPiece* pieceType, Color color);
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

	const bool IsPositionValid(const Position& position);

	// Player move methods
	void PlacePillar(uint16_t row, uint16_t column);
	void ProcessNextMove(Pillar& newPillar);
	void ValidateNewPillarPlacement(const Pillar& newPillar, Color playerColor);
	const std::vector<Bridge>& ProcessBridgesForNewPillar(const Pillar& newPillar);
	void UpdateAvailablePieces(const std::vector<Bridge>& newBridges, const Pillar& newPillar);
	void RemovePillar(uint16_t row, uint16_t column);

	// Check intersection methods
	bool CheckNoIntersections();
	bool Intersects(const Bridge& bridge1, const Bridge& bridge2);
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
};