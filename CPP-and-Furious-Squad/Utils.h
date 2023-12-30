#pragma once

#include <cstdint>

constexpr uint16_t MAX_AVAILABLE_PILLARS = 12;
constexpr uint16_t MAX_AVAILABLE_BRIDGES = 12;

enum class Color : uint8_t
{
	RED, BLACK, NO_COLOR
};