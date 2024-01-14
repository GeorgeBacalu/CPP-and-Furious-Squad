#pragma once

#include <vector>
#include <unordered_map>
#include "IPlayer.h"
#include "GameBoard.h"

class AiPlayer : public IPlayer {
	Color m_color;
	GameBoard& m_gameBoard;
	std::string m_policyName;
	std::vector<int64_t> m_previousStateActions;
	std::unordered_map<int64_t, float> m_stateActionCosts;

	std::vector<Position> GenerateActions(GameBoard& gameBoard);
	std::vector<Position> GenerateNonAdjacentActions(GameBoard& gameBoard);
	void SavePolicy() const;
	

	void SetPolicyName(std::string_view policyName);
public:
	AiPlayer(GameBoard& gameBoard, std::string_view policyName, Color color);
	~AiPlayer() override;

	Position GetNextAction() override;
	Position RandomAction();
	std::string_view GetName() const override;
	std::unordered_map<int64_t, float> GetStateActionCosts() const;

	void LoadPolicy();

	bool isPositionValid(const Position& position, GameBoard& gameBoard);

	void FreeReward(float target);

	bool IsAdjacentState(int64_t currentState, const std::vector<int64_t>& previousStates) const;
	Position ExtractPositionFromHash(int64_t stateHash) const;
	bool ArePositionsAdjacent(const Position& pos1, const Position& pos2) const;
};