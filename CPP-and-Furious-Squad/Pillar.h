#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <string_view>
#include "Bridge.h"
#include "Utils.h"

class Pillar
{
	std::pair<uint8_t, uint8_t> m_position;
	Color m_color : 1;
public:
	Pillar();
	Pillar(const std::pair<uint8_t, uint8_t>& position, Color color);
	Pillar(const Pillar& other);
	~Pillar();
	Pillar& operator=(const Pillar& other);
	friend std::ostream& operator<<(std::ostream& out, const Pillar& pillar);

	static std::string_view ToString(Color color);

	const Bridge& ConnectTo(const Pillar& other);
};