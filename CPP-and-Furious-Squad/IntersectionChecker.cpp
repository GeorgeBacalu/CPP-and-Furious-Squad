#include "IntersectionChecker.h"

IntersectionChecker::IntersectionChecker(GameBoardCore& gameBoardCore) : m_gameBoardCore{ gameBoardCore }
{
}

// Logic methods

bool IntersectionChecker::CheckNoIntersections()
{
	const auto& bridges = m_gameBoardCore.GetBridges();
	if (bridges.empty())
		return true;
	Bridge newBridge = bridges.back();
	for (const auto& existingBridge : bridges)
		if (Intersects(existingBridge, newBridge))
			return false;
	return true;
}

bool IntersectionChecker::Intersects(const Bridge& bridge1, const Bridge& bridge2)
{
	const auto& start1 = bridge1.GetStartPillar().GetPosition();
	const auto& end1 = bridge1.GetEndPillar().GetPosition();
	const auto& start2 = bridge2.GetStartPillar().GetPosition();
	const auto& end2 = bridge2.GetEndPillar().GetPosition();
	return (start1 == start2 || start1 == end2 || end1 == start2 || end1 == end2) && IntersectsOnSameAxis(bridge1, bridge2);
}

bool IntersectionChecker::IntersectsOnSameAxis(const Bridge& bridge1, const Bridge& bridge2)
{
	const auto& [startRow1, startColumn1] = bridge1.GetStartPillar().GetPosition();
	const auto& [endRow1, endColumn1] = bridge1.GetEndPillar().GetPosition();
	const auto& [startRow2, startColumn2] = bridge2.GetStartPillar().GetPosition();
	const auto& [endRow2, endColumn2] = bridge2.GetEndPillar().GetPosition();

	if (startRow1 == endRow1)
		return startRow2 == endRow2 && startRow1 == startRow2 && IntersectsOnAxis(startColumn1, endColumn1, startColumn2, endColumn2);
	else if (startColumn1 == endColumn1)
		return startColumn2 == endColumn2 && startColumn1 == startColumn2 && IntersectsOnAxis(startRow1, endRow1, startRow2, endRow2);
	return false;
}

bool IntersectionChecker::IntersectsOnAxis(size_t start1, size_t end1, size_t start2, size_t end2)
{
	return (start1 < end1) ? (start1 < start2 && start2 < end1) : (start1 > start2 && start2 > end1);
}