#pragma once

#include "Enums.h"
#include "Bridge.h"
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

class Pillar
{
	Color m_color;
	std::vector<Direction> m_peaks;
public:
	Pillar();
	Pillar(Color color, const std::vector<Direction>& peaks);
	Pillar(const Pillar& other);
	~Pillar();
	Pillar& operator=(const Pillar& other);
	friend std::ostream& operator<<(std::ostream& out, const Pillar& pillar);

	static std::string_view ToString(Color color);
	static std::string_view ToString(Direction direction);

	const Bridge& ConnectTo(const Pillar& other);
};