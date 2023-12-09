#pragma once

#include "IPlayer.h"

class HumanPlayer : public IPlayer {
	std::string m_name;
public:
	HumanPlayer(std::string_view name);

	Position GetNextAction() override;

	std::string_view GetName() const override;
};