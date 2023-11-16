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

const Bridge& Pillar::BuildBridgeTo(const Pillar& targetPillar)
{
	if (this->m_color != targetPillar.m_color)
		throw std::invalid_argument("Pillars must be of the same color to build a bridge!");
	bool isTargetPillarPositionValid = false;
	const std::vector<std::pair<int16_t, int16_t>> bridgeAllowedOffsets{ {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} };
	const auto& [currentRow, currentColumn] = this->m_position;
	const auto& [targetRow, targetColumn] = targetPillar.m_position;
	for (const auto& [offsetX, offsetY] : bridgeAllowedOffsets)
	{
		if (currentRow + offsetX >= BOARD_SIZE || currentColumn + offsetY >= BOARD_SIZE)
			break;
		if (currentRow == targetRow + offsetX && currentColumn == targetColumn + offsetY)
		{
			isTargetPillarPositionValid = true;
			break;
		}
	}
	if (!isTargetPillarPositionValid)
		throw std::invalid_argument("Target pillar is in an invalid position relative to the first one!");
	return Bridge(*this, targetPillar);
}