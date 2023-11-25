#pragma once

#include <cstdint>

constexpr uint16_t BOARD_SIZE = 24;
constexpr uint16_t MAX_SCORE = 25;
constexpr uint16_t MAX_AVAILABLE_PILLARS = 50;
constexpr uint16_t MAX_AVAILABLE_BRIDGES = 50;

enum class Color : uint8_t
{
	RED, BLACK
};