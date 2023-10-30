#pragma once

#include <cstdint>

constexpr uint8_t BOARD_SIZE = 24;
constexpr uint8_t MAX_SCORE = 25;

enum class Color : uint8_t
{
	RED, BLACK
};