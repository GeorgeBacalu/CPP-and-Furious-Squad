#pragma once
#include "Pillar.h"
class Bridge
{
	Pillar m_startPillar;
	Pillar m_endPillar;

public :

	Bridge();
	Bridge(const Pillar& startPillar, const Pillar& endPillar);
	Bridge(const Bridge& other);
	~Bridge();

	Bridge& operator=(const Bridge& other);
	friend std::ostream& operator<<(std::ostream& out, const Bridge& bridge);

	bool isValidBetween(const Pillar& startPillar, const Pillar& endPillar);
};

