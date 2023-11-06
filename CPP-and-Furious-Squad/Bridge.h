#pragma once

#include "Pillar.h"

class Bridge
{
	Pillar m_startPillar;
	Pillar m_endPillar;

	bool CheckSameColor(const Bridge& bridge);
	bool CheckDistinctPositions(const Bridge& bridge);
	bool CheckBridgeValid(const Bridge& bridge);
	bool CheckNoIntersections(const Bridge& bridge);
public:
	Bridge();
	Bridge(const Pillar& startPillar, const Pillar& endPillar);
	Bridge(const Bridge& other);
	Bridge(Bridge&& other) noexcept;
	Bridge& operator=(const Bridge& other);
	Bridge& operator=(Bridge&& other) noexcept;
	~Bridge() = default;
	friend std::istream& operator>>(std::istream& in, Bridge& bridge);
	friend std::ostream& operator<<(std::ostream& out, const Bridge& bridge);

	const Pillar& GetStartPillar() const;
	const Pillar& GetEndPillar() const;

	bool IsValid(const Bridge& bridge);
};