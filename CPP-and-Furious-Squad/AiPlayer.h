#pragma once

#include <vector>
#include <unordered_map>
#include "IPlayer.h"
#include "GameBoard.h"

class AiPlayer : public IPlayer {
	GameBoard& m_gameBoard;
	std::string m_policyName;
	std::vector<int64_t> m_previousStateActions;
	std::unordered_map<int64_t, float> m_stateActionCosts;

	static std::vector<Position> GenerateActions(const GameBoard& gameBoard);
	void SavePolicy() const;
	void LoadPolicy();
public:
	AiPlayer(GameBoard& gameBoard, std::string_view policyName);
	~AiPlayer() override;

	const Position& GetNextAction() override;

	std::string_view GetName() const override;

	void FreeReward(float target);
};