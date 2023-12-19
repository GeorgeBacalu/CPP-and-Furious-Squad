#pragma once
#include "Bridge.h"
#include "GameBoardCore.h"
#include "IntersectionChecker.h"
#include <vector>

class GameController
{
	GameBoardCore& m_gameBoardCore;
	IntersectionChecker& m_intersectionChecker;
	bool m_playerTurn{ true };
	bool m_invalid{ false };
	std::vector<Pillar> m_redPillars;
	std::vector<Pillar> m_blackPillars;
	std::vector<Pillar> m_endPillars;
public:
	GameController(GameBoardCore& gameBoardCore, IntersectionChecker& intersectionChecker);
	GameController(const GameController& other) = delete;
	GameController& operator=(const GameController& other) = delete;
	~GameController() = default;

	// Getters and Setters
	bool GetPlayerTurn() const;
	bool GetInvalid() const;
	std::vector<Pillar> GetRedPillars() const;
	std::vector<Pillar> GetBlackPillars() const;
	std::vector<Pillar> GetEndPillars() const;
	uint16_t GetAvailablePieces(IPiece* pieceType, Color color);
	void SetPlayerTurn(bool playerTurn);
	void SetInvalid(bool invalid);
	void SetRedPillars(const std::vector<Pillar>& redPillars);
	void SetBlackPillars(const std::vector<Pillar>& blackPillars);
	void SetEndPillars(const std::vector<Pillar>& endPillars);

	// Logic methods
	void SwitchPlayerTurn();
	void InitEndPillars();
	bool CheckWin(Color playerColor);

	// Player move methods
	void PlacePillar(uint16_t row, uint16_t column);
	void ProcessNextMove(Pillar& newPillar);
	void ValidateNewPillarPlacement(const Pillar& newPillar, Color playerColor);
	const std::vector<Bridge>& ProcessBridgesForNewPillar(const Pillar& newPillar);
	void UpdateAvailablePieces(const std::vector<Bridge>& newBridges, const Pillar& newPillar);
	void RemovePillar(uint16_t row, uint16_t column);

	// Game flow methods
	void ResetGame();

	// Related to AI player
	int64_t GetHashWithPosition(const Position& position) const;
};