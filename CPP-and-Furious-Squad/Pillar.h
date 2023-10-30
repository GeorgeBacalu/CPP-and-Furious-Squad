#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <string_view>
#include <unordered_map>
#include "Utils.h"

using Point = std::pair<uint8_t, uint8_t>;

class Bridge;

class Pillar
{
	Point m_position;
	Color m_color : 1;
public:
	Pillar();
	Pillar(const Point& position, Color color);
	Pillar(const Pillar& other);
	~Pillar();
	Pillar& operator=(const Pillar& other);
	friend std::istream& operator>>(std::istream& in, Pillar& pillar);
	friend std::ostream& operator<<(std::ostream& out, const Pillar& pillar);

	Bridge BuildBridgeTo(const Pillar& other);
};