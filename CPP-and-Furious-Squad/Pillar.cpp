#include "Pillar.h"
#include "Bridge.h"

Color Pillar::GetRandomColor()
{
	return rand() % 2 ? Color::RED : Color::BLACK;
}

Pillar::Pillar() : m_position{ 0, 0 }, m_color{ GetRandomColor() }
{
}

Pillar::Pillar(const Position& position, Color color) : m_position{ position }, m_color{ color }
{
}

Pillar::Pillar(const Pillar& other) : Pillar{ other.m_position, other.m_color }
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

Pillar::Pillar(Pillar&& other) noexcept : Pillar{ std::move(other.m_position), other.m_color }
{
	other.m_position = { 0, 0 };
	other.m_color = GetRandomColor();
}

Pillar& Pillar::operator=(Pillar&& other) noexcept
{
	if (this != &other)
	{
		m_position = std::move(other.m_position);
		m_color = other.m_color;
		other.m_position = { 0, 0 };
		other.m_color = GetRandomColor();
	}
	return *this;
}

std::istream& operator>>(std::istream& in, Pillar& pillar)
{
	static const std::unordered_map<std::string, Color> colorMap{ {"RED", Color::RED}, {"BLACK", Color::BLACK} };
	std::string colorString;
	if (!(in >> pillar.m_position.first >> pillar.m_position.second >> colorString)) {
		pillar.m_position = { 0, 0 };
		throw std::invalid_argument("Error reading pillar input!");
	}
	const auto& [row, column] = pillar.m_position;
	if (row >= BOARD_SIZE || column >= BOARD_SIZE || colorMap.find(colorString) == colorMap.end())
		throw std::invalid_argument("Invalid pillar input!");
	pillar.m_color = colorMap.at(colorString);
	return in;
}

std::ostream& operator<<(std::ostream& out, const Pillar& pillar)
{
	const auto& [row, column] = pillar.m_position;
	return out << "Position: (" << row << ", " << column << "), Color: " << (static_cast<int>(pillar.m_color) == 0 ? "RED" : "BLACK") << "\n";
}

bool Pillar::operator==(const Pillar& other) const
{
	return m_position == other.m_position && m_color == other.m_color;
}

const Position& Pillar::GetPosition() const
{
	return m_position;
}

void Pillar::SetPosition(const Position& position)
{
	const auto& [row, column] = position;
	if (row >= BOARD_SIZE && column >= BOARD_SIZE)
		throw std::out_of_range("Invalid position values!");
	m_position = position;
}

void Pillar::SetPosition(uint16_t x, uint16_t y)
{
	SetPosition({ x, y });
}

Color Pillar::GetColor() const
{
	return m_color;
}

void Pillar::SetColor(Color color)
{
	m_color = color;
}