#include "Bridge.h"

Color Bridge::GetRandomColor()
{
	return rand() % 2 == 1 ? Color::RED : Color::BLACK;
}


Bridge::Bridge() : m_startPillar{ Pillar() }, m_endPillar{ Pillar() }, m_color{ GetRandomColor() }
{
}

Bridge::Bridge(const Pillar& startPillar, const Pillar& endPillar, Color color) : m_startPillar{ startPillar }, m_endPillar(endPillar), m_color{ color }
{
}

Bridge::Bridge(const Bridge& other) : m_startPillar{ other.m_startPillar }, m_endPillar(other.m_endPillar)
{
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

Bridge& Bridge::operator=(const Bridge& other)
{
	if (this != &other)
	{
		this->m_startPillar = other.m_startPillar;
		this->m_endPillar = other.m_endPillar;
	}

	return*this;
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

	return*this;
}

Bridge::Bridge(Bridge&& other) noexcept : m_startPillar{ std::move(other.m_startPillar) }, m_endPillar{ std::move(other.m_endPillar) }, m_color{ other.m_color }
{
	other.m_startPillar = Pillar();
	other.m_endPillar = Pillar();
	other.m_color = GetRandomColor();
}

std::istream& operator>>(std::istream& in, Bridge& bridge)
{
	if (!(in >> bridge.m_startPillar >> bridge.m_endPillar))
	{
		throw std::invalid_argument("Error while reading bridge input.");
	}

	if (!bridge.IsValid(bridge))
	{
		throw std::invalid_argument("Can't build bridge between this pillars.");
	}

	return in;
}

std::ostream& operator<<(std::ostream& out, const Bridge& bridge)
{
	out << "Start pillar : " << bridge.m_startPillar << std::endl;
	out << "End pillar : " << bridge.m_endPillar << std::endl;

	return out;
}

bool Bridge::IsValid(const Bridge& bridge)
{
	return CheckSameColor(bridge) && CheckDistinctPositions(bridge) && CheckBridgeValid(bridge) && CheckNoIntersections(bridge);
}

bool Bridge::CheckSameColor(const Bridge& bridge)
{
	throw std::logic_error("Method not yet implemented!");
}

bool Bridge::CheckDistinctPositions(const Bridge& bridge)
{
	throw std::logic_error("Method not yet implemented!");
}

bool Bridge::CheckBridgeValid(const Bridge& bridge)
{
	throw std::logic_error("Method not yet implemented!");
}

bool Bridge::CheckNoIntersections(const Bridge& bridge)
{
	throw std::logic_error("Method not yet implemented!");
}