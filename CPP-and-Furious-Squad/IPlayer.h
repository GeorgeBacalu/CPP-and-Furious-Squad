#pragma once

#include "Pillar.h"

struct IPlayer {
	virtual ~IPlayer() = default;

	virtual Position GetNextAction() = 0;
	virtual std::string_view GetName() const = 0;
};