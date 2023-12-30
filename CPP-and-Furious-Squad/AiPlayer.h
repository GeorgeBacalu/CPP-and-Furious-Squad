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
	void SavePolicy() const;
	void LoadPolicy();

	void SetPolicyName(std::string_view policyName);
public:
	AiPlayer(GameBoard& gameBoard, std::string_view policyName, Color color);
	~AiPlayer() override;

	Position GetNextAction() override;
	std::string_view GetName() const override;

	bool isPositionValid(const Position& position, GameBoard& gameBoard);

	void FreeReward(float target);
};