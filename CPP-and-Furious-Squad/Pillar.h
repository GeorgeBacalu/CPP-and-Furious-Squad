#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include "IPiece.h"

using Position = std::pair<size_t, size_t>;

class Bridge;

class Pillar : public IPiece
{
public:
	static const size_t kWidth{ 4 };
	static const size_t kHeight{ 4 };
private:
	Position m_position;
	Color m_color : 2;
public:
	Pillar();
	Pillar(Color color);
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
	void SetPosition(size_t row, size_t column);
	Color GetColor() const override;
	void SetColor(Color color);
};