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
	out << "Pillar color: " << (pillar.m_color == Color::RED ? "RED" : "BLACK");
	out << "\nConnected peaks: ";
	for (const Direction& direction : pillar.m_peaks)
	{
		switch (direction)
		{
		case Direction::NORTH: out << "NORTH "; break;
		case Direction::SOUTH: out << "SOUTH "; break;
		case Direction::EAST: out << "EAST "; break;
		case Direction::WEST: out << "WEST "; break;
		}
	}
	return out;
}

void Pillar::ConnectTo(const Pillar& other)
{
	throw std::logic_error("Method not yet implemented!");
}