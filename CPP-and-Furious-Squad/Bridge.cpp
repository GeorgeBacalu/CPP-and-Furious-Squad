#include "Bridge.h"

Color Bridge::GetRandomColor()
{
	return rand() % 2 ? Color::RED : Color::BLACK;
}

Bridge::Bridge() : m_startPillar{ Pillar() }, m_endPillar{ Pillar() }, m_color{ GetRandomColor() }
{
}

Bridge::Bridge(const Pillar& startPillar, const Pillar& endPillar, Color color) : m_startPillar{ startPillar }, m_endPillar{ endPillar }, m_color{ color }
{
}

Bridge::Bridge(const Bridge& other) : m_startPillar{ other.m_startPillar }, m_endPillar{ other.m_endPillar }
{
}

Bridge& Bridge::operator=(const Bridge& other)
{
	if (this != &other)
	{
		this->m_startPillar = other.m_startPillar;
		this->m_endPillar = other.m_endPillar;
		this->m_color = other.m_color;
	}
	return *this;
}

Bridge::Bridge(Bridge&& other) noexcept : m_startPillar{ std::move(other.m_startPillar) }, m_endPillar{ std::move(other.m_endPillar) }, m_color{ other.m_color }
{
	other.m_startPillar = Pillar();
	other.m_endPillar = Pillar();
	other.m_color = GetRandomColor();
}

Bridge& Bridge::operator=(Bridge&& other) noexcept
{
	if (this != &other)
	{
		m_startPillar = std::move(other.m_startPillar);
		m_endPillar = std::move(other.m_endPillar);
		m_color = other.m_color;
		other.m_startPillar = Pillar();
		other.m_endPillar = Pillar();
		other.m_color = GetRandomColor();
	}
	return *this;
}

std::istream& operator>>(std::istream& in, Bridge& bridge)
{
	static const std::unordered_map<std::string, Color> s_colorMap = { {"RED", Color::RED}, {"BLACK", Color::BLACK} };
	std::string colorString;
	if (!(in >> bridge.m_startPillar >> bridge.m_endPillar >> colorString))
	{
		bridge.m_startPillar = Pillar();
		bridge.m_endPillar = Pillar();
		throw std::invalid_argument("Error while reading bridge input.");
	}
	if (s_colorMap.find(colorString) == s_colorMap.end())
		throw std::invalid_argument("Invalid color string!");
	bridge.m_color = s_colorMap.at(colorString);
	if (!bridge.IsValid(bridge))
		throw std::invalid_argument("Can't build bridge between this pillars.");
	return in;
}

std::ostream& operator<<(std::ostream& out, const Bridge& bridge)
{
	return out << "Start pillar: " << bridge.m_startPillar << "End Pillar: " << bridge.m_endPillar << (static_cast<int>(bridge.m_color) == 0 ? "RED" : "BLACK") << "\n";
}

const Pillar& Bridge::GetStartPillar() const
{
	return m_startPillar;
}

const Pillar& Bridge::GetEndPillar() const
{
	return m_endPillar;
}

Color Bridge::GetColor() const
{
	return m_color;
}

bool Bridge::IsValid(const Bridge& bridge)
{
	return CheckSameColor(bridge) && CheckDistinctPositions(bridge) && CheckBridgeValid(bridge) && CheckNoIntersections(bridge);
}

bool Bridge::CheckSameColor(const Bridge& bridge)
{
	return true;
}

bool Bridge::CheckDistinctPositions(const Bridge& bridge)
{
	return true;
}

bool Bridge::CheckBridgeValid(const Bridge& bridge)
{
	return true;
}

bool Bridge::CheckNoIntersections(const Bridge& bridge)
{
	return true;
}