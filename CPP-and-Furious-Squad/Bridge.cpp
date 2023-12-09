#include "Bridge.h"

static Pillar pillar;

Bridge::Bridge() : m_startPillar{ pillar }, m_endPillar{ pillar }
{
}

Bridge::Bridge(Pillar& startPillar, Pillar& endPillar) : m_startPillar{ startPillar }, m_endPillar{ endPillar }
{
	if (!IsValid())
		throw std::invalid_argument("Can't build bridge between this pillars.");
}

Bridge::Bridge(const Bridge& other) : Bridge{ other.m_startPillar, other.m_endPillar }
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

std::istream& operator>>(std::istream& in, Bridge& bridge)
{
	if (!(in >> bridge.m_startPillar >> bridge.m_endPillar))
	{
		bridge.m_startPillar = Pillar();
		bridge.m_endPillar = Pillar();
		throw std::invalid_argument("Error while reading bridge input.");
	}
	if (!bridge.IsValid())
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

Color Bridge::GetColor() const
{
	return CheckSameColor() ? m_startPillar.GetColor() : Color::NO_COLOR;
}

bool Bridge::operator==(const Bridge& other) const
{
	return m_startPillar == other.m_startPillar && m_endPillar == other.m_endPillar;
}

bool Bridge::IsValid() const
{
	return CheckSameColor() && CheckDistinctPositions() && CheckBridgeValid();
}

bool Bridge::CheckSameColor() const
{
	return m_startPillar.GetColor() == m_endPillar.GetColor();
}

bool Bridge::CheckDistinctPositions() const
{
	const auto& [startRow, startColumn] = m_startPillar.GetPosition();
	const auto& [endRow, endColumn] = m_endPillar.GetPosition();
	return startRow != endRow && startColumn != endColumn;
}

bool Bridge::CheckBridgeValid() const
{
	const std::vector<std::pair<int16_t, int16_t>> bridgeAllowedOffsets{ {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} };
	for (const auto& [offsetX, offsetY] : bridgeAllowedOffsets)
	{
		const auto& [startPillarRow, startPillarColumn] = m_startPillar.GetPosition();
		const auto& [endPillarRow, endPillarColumn] = m_endPillar.GetPosition();
		if (startPillarRow == endPillarRow + offsetX && startPillarColumn == endPillarColumn + offsetY)
			return true;
	}
	return false;
}