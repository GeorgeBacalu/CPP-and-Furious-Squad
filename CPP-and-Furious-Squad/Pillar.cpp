#include "Pillar.h"

Pillar::Pillar() : m_position{ 0, 0 }, m_color { rand() % 2 ? Color::RED : Color::BLACK } 
{
}

Pillar::Pillar(const std::pair<uint8_t, uint8_t>& position, Color color) : m_position{ position }, m_color { color } 
{
}

Pillar::Pillar(const Pillar& other) : m_position{ other.m_position }, m_color{ other.m_color } 
{
}

Pillar::~Pillar()
{
}

Pillar& Pillar::operator=(const Pillar& other)
{
	if (this != &other)
	{
		m_position = other.m_position;
		m_color = other.m_color;
	}
	return *this;
}

std::ostream& operator<<(std::ostream& out, const Pillar& pillar)
{
	const auto& [x, y] = pillar.m_position;
	return out << "Position: (" << x << ", " << y << "), Color: " << (static_cast<int>(pillar.m_color) == 0 ? "RED" : "BLACK") << "\n";
}

Bridge Pillar::BuildBridgeTo(const Pillar& other)
{
	throw std::logic_error("Method not yet implemented!");
}