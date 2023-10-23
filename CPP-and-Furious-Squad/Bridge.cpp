#include "Bridge.h"

Bridge::Bridge() : m_startPillar{ Pillar() }, m_endPillar{ Pillar() }
{
}

Bridge::Bridge(const Pillar& startPillar, const Pillar& endPillar) : m_startPillar{ startPillar }, m_endPillar(endPillar)
{
}

Bridge::Bridge(const Bridge& other) : m_startPillar{ other.m_startPillar }, m_endPillar(other.m_endPillar)
{
}

Bridge::~Bridge()
{
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

std::ostream& operator<<(std::ostream& out, const Bridge& bridge)
{
	out << "Start pillar : " << bridge.m_startPillar << std::endl;
	out << "End pillar : " << bridge.m_endPillar << std::endl;

	return out;
}

bool Bridge::isValidBetween(const Pillar& startPillar, const Pillar& endPillar)
{
	throw std::logic_error("Method not yet implemented!");
}