#pragma once
#include "Bridge.h"
#include "GameBoardCore.h"

class IntersectionChecker
{
	GameBoardCore& m_gameBoardCore;
public:
	IntersectionChecker(GameBoardCore& gameBoardCore);
	IntersectionChecker(const IntersectionChecker& other) = delete;
	IntersectionChecker& operator=(const IntersectionChecker& other) = delete;
	~IntersectionChecker() = default;

	// Logic methods
	bool CheckNoIntersections();
	bool Intersects(const Bridge& bridge1, const Bridge& bridge2);
	bool IntersectsOnSameAxis(const Bridge& bridge1, const Bridge& bridge2);
	bool IntersectsOnAxis(size_t start1, size_t end1, size_t start2, size_t end2);
};