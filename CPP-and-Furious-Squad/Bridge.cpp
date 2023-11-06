#include "Bridge.h"


Bridge::Bridge() : m_startPillar{ Pillar() }, m_endPillar{ Pillar() }
{
}

Bridge::Bridge(const Pillar& startPillar, const Pillar& endPillar) : m_startPillar{ startPillar }, m_endPillar{ endPillar }
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
	}
	return *this;
}

Bridge::Bridge(Bridge&& other) noexcept : m_startPillar{ std::move(other.m_startPillar) }, m_endPillar{ std::move(other.m_endPillar) }
{
	other.m_startPillar = Pillar();
	other.m_endPillar = Pillar();
}

Bridge& Bridge::operator=(Bridge&& other) noexcept
{
	if (this != &other)
	{
		m_startPillar = std::move(other.m_startPillar);
		m_endPillar = std::move(other.m_endPillar);
		other.m_startPillar = Pillar();
		other.m_endPillar = Pillar();
	}
	return *this;
}

std::istream& operator>>(std::istream& in, Bridge& bridge)
{
	if (!(in >> bridge.m_startPillar >> bridge.m_endPillar))
	{
		bridge.m_startPillar = Pillar();
		bridge.m_endPillar = Pillar();
		throw std::invalid_argument("Error while reading bridge input.");
	}
	if (!bridge.IsValid(bridge))
		throw std::invalid_argument("Can't build bridge between this pillars.");
	return in;
}

std::ostream& operator<<(std::ostream& out, const Bridge& bridge)
{
	return out << "Start pillar: " << bridge.m_startPillar << "End Pillar: " << bridge.m_endPillar << "\n";
}

const Pillar& Bridge::GetStartPillar() const
{
	return m_startPillar;
}

const Pillar& Bridge::GetEndPillar() const
{
	return m_endPillar;
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