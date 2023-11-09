#include "Pillar.h"
#include "Bridge.h"
#include <optional>

Color Pillar::GetRandomColor()
{
	return rand() % 2 ? Color::RED : Color::BLACK;
}

Pillar::Pillar() : m_position{ 0, 0 }, m_color{ GetRandomColor() }
{
}

Pillar::Pillar(const Point& position, Color color) : m_position{ position }, m_color{ color }
{
}

Pillar::Pillar(const Pillar& other) : m_position{ other.m_position }, m_color{ other.m_color }
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

Pillar::Pillar(Pillar&& other) noexcept : m_position{ std::move(other.m_position) }, m_color{ other.m_color }
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
	static const std::unordered_map<std::string, Color> s_colorMap = { {"RED", Color::RED}, {"BLACK", Color::BLACK} };
	std::string colorString;
	if (!(in >> pillar.m_position.first >> pillar.m_position.second >> colorString)) {
		pillar.m_position = { 0, 0 };
		throw std::invalid_argument("Error reading pillar input!");
	}
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

bool Pillar::operator==(const Pillar& other) const
{
	return m_position == other.m_position && m_color == other.m_color;
}



const Point& Pillar::GetPosition() const
{
	return m_position;
}

void Pillar::SetPosition(const Point& position)
{
	if (position.first >= BOARD_SIZE && position.second >= BOARD_SIZE)
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

Bridge Pillar::BuildBridgeTo(const Pillar& targetPillar)
{
	if (this->m_color != targetPillar.m_color)
		throw std::invalid_argument("Pillars must be of the same color to build a bridge!");
	bool isTargetPillarPositionValid = false;
	const std::vector<std::pair<int16_t, int16_t>> bridgeAllowedOffsets = {
		{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
		{1, 2}, {1, -2}, {-1, 2}, {-1, -2}
	};
	for (const auto& [offsetX, offsetY] : bridgeAllowedOffsets) {
		if (this->m_position.first == targetPillar.m_position.first + offsetX && this->m_position.second == targetPillar.m_position.second + offsetY) {
			isTargetPillarPositionValid = true;
			break;
		}
	}
	if (!isTargetPillarPositionValid)
		throw std::invalid_argument("Target pillar is in an invalid position relative to the first one!");
	Bridge bridge = Bridge(*this, targetPillar);
	return bridge;
}