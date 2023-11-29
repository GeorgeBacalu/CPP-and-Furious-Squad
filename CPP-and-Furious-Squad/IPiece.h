#pragma once

#include "Utils.h"

struct IPiece {
public:
	IPiece() = default;
	IPiece(const IPiece& other) = default;
	IPiece& operator=(const IPiece& other) = default;
	IPiece(IPiece&& other) noexcept = default;
	IPiece& operator=(IPiece&& other) noexcept = default;
	virtual ~IPiece() = default;

	virtual Color GetColor() const = 0;
};