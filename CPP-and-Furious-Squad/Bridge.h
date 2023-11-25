#pragma once

#include "Pillar.h"

class Bridge : public IPiece
{
	Pillar m_startPillar;
	Pillar m_endPillar;

	bool CheckSameColor();
	bool CheckDistinctPositions();
	bool CheckBridgeValid();
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
	Color GetColor() const override;
	bool operator==(const Bridge& other) const;

	bool IsValid();
};