#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include "IPiece.h"

using Position = std::pair<uint16_t, uint16_t>;

class Bridge;

class Pillar : public IPiece
{
	Position m_position;
	Color m_color : 1;

	static Color GetRandomColor();
public:
	Pillar();
	Pillar(const Position& position, Color color);
	Pillar(const Pillar& other);
	Pillar& operator=(const Pillar& other);
	Pillar(Pillar&& other) noexcept;
	Pillar& operator=(Pillar&& other) noexcept;
	~Pillar() = default;
	friend std::istream& operator>>(std::istream& in, Pillar& pillar);
	friend std::ostream& operator<<(std::ostream& out, const Pillar& pillar);
	bool operator==(const Pillar& other) const;

	const Position& GetPosition() const;
	void SetPosition(const Position& position);
	void SetPosition(uint16_t row, uint16_t column);
	Color GetColor() const override;
	void SetColor(Color color);
};