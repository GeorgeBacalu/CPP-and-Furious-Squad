#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
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

	const Point& GetPosition() const;
	void SetPosition(const Point& position);
	void SetPosition(uint8_t x, uint8_t y);
	Color GetColor() const;
	void SetColor(Color color);

	Bridge BuildBridgeTo(const Pillar& other);
};