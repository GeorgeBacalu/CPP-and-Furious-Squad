#pragma once

#include "Enums.h"
#include <iostream>
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

	void ConnectTo(const Pillar& other);
};