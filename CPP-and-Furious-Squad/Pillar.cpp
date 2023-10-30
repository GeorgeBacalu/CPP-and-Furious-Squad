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

std::istream& operator>>(std::istream& in, Pillar& pillar) {
	const std::unordered_map<std::string, Color> s_colorMap = { {"RED", Color::RED}, {"BLACK", Color::BLACK} };
	std::string colorString;
	if (!(in >> pillar.m_position.first >> pillar.m_position.second >> colorString))
		throw std::invalid_argument("Error reading pillar input!");
	const auto& [x, y] = pillar.m_position;
	if (x >= BOARD_SIZE || y >= BOARD_SIZE || s_colorMap.find(colorString) == s_colorMap.end())
		throw std::invalid_argument("Invalid pillar input!");
	pillar.m_color = s_colorMap.at(colorString);
	return in;
}

std::ostream& operator<<(std::ostream& out, const Pillar& pillar)
{
	const auto& [x, y] = pillar.m_position;
	return out << "Position: (" << x << ", " << y << "), Color: " << (static_cast<int>(pillar.m_color) == 0 ? "RED" : "BLACK") << "\n";
}

const Point& Pillar::GetPosition() const 
{ 
	return m_position; 
}

void Pillar::SetPosition(const Point& position) 
{ 
	m_position = position;
}

void Pillar::SetPosition(uint8_t x, uint8_t y) 
{ 
	m_position = { x, y };
}

Color Pillar::GetColor() const 
{ 
	return m_color;
}

void Pillar::SetColor(Color color) 
{ 
	m_color = color;
}

Bridge Pillar::BuildBridgeTo(const Pillar& other)
{
	throw std::logic_error("Method not yet implemented!");
}