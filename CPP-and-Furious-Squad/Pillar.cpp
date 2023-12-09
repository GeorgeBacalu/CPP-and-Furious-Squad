#include "Pillar.h"
#include "Bridge.h"

Pillar::Pillar() : m_color{ Color::NO_COLOR }
{
}

Pillar::Pillar(Color color) : m_color{ color }
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
	other.m_color = Color::NO_COLOR;
}

Pillar& Pillar::operator=(Pillar&& other) noexcept
{
	if (this != &other)
	{
		m_position = std::move(other.m_position);
		m_color = other.m_color;
		other.m_position = { 0, 0 };
		other.m_color = Color::NO_COLOR;
	}
	return *this;
}

std::istream& operator>>(std::istream& in, Pillar& pillar)
{
	static const std::unordered_map<std::string, Color> colorMap{ {"RED", Color::RED}, {"BLACK", Color::BLACK} };
	std::string colorString;
	if (!(in >> pillar.m_position.first >> pillar.m_position.second >> colorString))
	{
		pillar.m_position = { 0, 0 };
		throw std::invalid_argument("Error reading pillar input!");
	}
	const auto& [row, column] = pillar.m_position;
	if (row >= Pillar::kHeight || column >= Pillar::kWidth || colorMap.find(colorString) == colorMap.end())
		throw std::invalid_argument("Invalid pillar input!");
	pillar.m_color = colorMap.at(colorString);
	return in;
}

std::ostream& operator<<(std::ostream& out, const Pillar& pillar)
{
	std::unordered_map<Color, std::string> colorMap{ {Color::RED, "RED"}, {Color::BLACK, "BLACK"} };
	const auto& [row, column] = pillar.m_position;
	return out << "Position: (" << row << ", " << column << "), Color: " << colorMap[pillar.m_color] << "\n";
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
	if (row > Pillar::kHeight || column > Pillar::kWidth)
		throw std::out_of_range("Invalid position values!");
	m_position = position;
}

void Pillar::SetPosition(size_t row, size_t column)
{
	SetPosition({ row, column });
}

Color Pillar::GetColor() const
{
	return m_color;
}

void Pillar::SetColor(Color color)
{
	m_color = color;
}