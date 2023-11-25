#pragma once

#include <unordered_map>
#include "Pillar.h"
#include "Bridge.h"

class AvailablePieces {
	std::unordered_map<uint16_t, Pillar> m_availableRedPillars;
	std::unordered_map<uint16_t, Bridge> m_availableRedBridges;
	std::unordered_map<uint16_t, Pillar> m_availableBlackPillars;
	std::unordered_map<uint16_t, Bridge> m_availableBlackBridges;

	// before game starts
	void GeneratePieces();
	void EmplacePillar(Pillar&& pillar); // emplaces the initial pillars on the side
	void EmplaceBridge(Bridge&& bridge); // emplaces the initial bridges on the side
public:
	AvailablePieces();
	friend std::ostream& operator<<(std::ostream& out, const AvailablePieces& availablePieces);

	const Pillar& PickPillar(const Pillar& pillar, uint16_t key);
	const Bridge& PickBridge(const Bridge& bridge, uint16_t key);
};