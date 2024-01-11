#pragma once

#include "Pillar.h"

class Bridge : public IPiece
{
	Pillar m_startPillar;
	Pillar m_endPillar;

	bool CheckSameColor() const;
	bool CheckDistinctPositions() const;
	bool CheckBridgeValid() const;
public:
	Bridge();
	Bridge(const Pillar& startPillar, const Pillar& endPillar);
	Bridge(const Bridge& other);
	Bridge& operator=(const Bridge& other);
	Bridge reverse() const;

	Bridge(Bridge&& other) noexcept = default;
	Bridge& operator=(Bridge&& other) noexcept = default;
	~Bridge() = default;
	friend std::istream& operator>>(std::istream& in, Bridge& bridge);
	friend std::ostream& operator<<(std::ostream& out, const Bridge& bridge);

	const Pillar& GetStartPillar() const;
	const Pillar& GetEndPillar() const;
	Color GetColor() const override;
	bool operator==(const Bridge& other) const;
	bool IsValid() const;
};