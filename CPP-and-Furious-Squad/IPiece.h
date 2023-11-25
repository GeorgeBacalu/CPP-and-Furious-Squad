#pragma once

#include "Utils.h"

struct IPiece {
	virtual ~IPiece() = default;

	virtual Color GetColor() const = 0;
};