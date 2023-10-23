#include "Pillar.h"

Pillar::Pillar() : m_color{ Color::RED }
{
}

Pillar::Pillar(Color color, const std::vector<Direction>& peaks) : m_color{ color }, m_peaks{ peaks }
{
}

Pillar::Pillar(const Pillar& other) : m_color{ other.m_color }, m_peaks{ other.m_peaks }
{
}

Pillar::~Pillar()
{
}

Pillar& Pillar::operator=(const Pillar& other)
{
	if (this != &other)
	{
		m_color = other.m_color;
		m_peaks = other.m_peaks;
	}
	return *this;
}

std::ostream& operator<<(std::ostream& out, const Pillar& pillar)
{
	out << "Pillar color: " << Pillar::ToString(pillar.m_color);
	out << "\nConnected peaks: ";
	for (const Direction& direction : pillar.m_peaks)
		out << Pillar::ToString(direction) << " ";
	return out;
}

std::string_view Pillar::ToString(Color color)
{
	return color == Color::RED ? "RED" : "BLACK";
}

std::string_view Pillar::ToString(Direction direction)
{
	switch (direction)
	{
	case Direction::NORTH: return "NORTH";
	case Direction::SOUTH: return "SOUTH";
	case Direction::EAST: return "EAST";
	case Direction::WEST: return "WEST";
	default: throw std::invalid_argument("Invalid Direction value!");
	}
}

const Bridge& Pillar::ConnectTo(const Pillar& other)
{
	throw std::logic_error("Method not yet implemented!");
}