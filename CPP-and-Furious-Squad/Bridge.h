#pragma once
#include "Pillar.h"
class Bridge
{
	Pillar m_startPillar;
	Pillar m_endPillar;
	Color m_color : 1;

	static Color GetRandomColor();

	bool CheckSameColor(const Bridge& bridge);
	bool CheckDistinctPositions(const Bridge& bridge);
	bool CheckBridgeValid(const Bridge& bridge);
	bool CheckNoIntersections(const Bridge& bridge);

public :

	Bridge();
	Bridge(const Pillar& startPillar, const Pillar& endPillar, Color color);
	Bridge(const Bridge& other);
	~Bridge();

	const Pillar& GetStartPillar() const;
	const Pillar& GetEndPillar() const;
	Color GetColor() const;

	Bridge& operator=(const Bridge& other);
	friend std::istream& operator>>(std::istream& in, Bridge& bridge);
	friend std::ostream& operator<<(std::ostream& out, const Bridge& bridge);

	bool IsValid(const Bridge& bridge);
};

