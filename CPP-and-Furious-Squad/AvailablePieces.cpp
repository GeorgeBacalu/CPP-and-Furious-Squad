#include "AvailablePieces.h"

using enum Color;

void AvailablePieces::GeneratePieces()
{
    for (size_t i = 0; i < MAX_AVAILABLE_PILLARS; ++i) 
    {
        Pillar redPillar{ RED };
        EmplacePillar(std::move(redPillar));
    }
    for (size_t i = 0; i < MAX_AVAILABLE_PILLARS; ++i) 
    {
        Pillar blackPillar{ BLACK };
        EmplacePillar(std::move(blackPillar));
    }
}

void AvailablePieces::EmplacePillar(Pillar&& pillar)
{
    uint16_t key = 0;
    if (pillar.GetColor() == RED)
    {
        m_availableRedPillars.emplace(++key, std::forward<Pillar>(pillar));
    }
    else
    {
        m_availableBlackPillars.emplace(++key, std::forward<Pillar>(pillar));
    }
}

void AvailablePieces::EmplaceBridge(Bridge&& bridge)
{
    throw std::logic_error("Method not yet implemented");
}

AvailablePieces::AvailablePieces() 
{
    GeneratePieces();
}

const Pillar& AvailablePieces::PickPillar(const Pillar& pillar, uint16_t key)
{
    if (pillar.GetColor() == RED)
    {
        if (auto itMap{ m_availableRedPillars.find(key) }; itMap != m_availableRedPillars.end())
        {
            const auto& pillar{ std::move(itMap->second) };
            m_availableRedPillars.erase(itMap);
            return pillar;
        }
    }
    else
    {
        if (auto itMap{ m_availableBlackPillars.find(key) }; itMap != m_availableBlackPillars.end())
        {
            const auto& pillar{ std::move(itMap->second) };
            m_availableBlackPillars.erase(itMap);
            return pillar;
        }
    }
}

const Bridge& AvailablePieces::PickBridge(const Bridge& bridge, uint16_t key)
{
    throw std::logic_error("Method not yet implemented");
}

std::ostream& operator<<(std::ostream& out, const AvailablePieces& availablePieces)
{
    for (const auto& [key, value] : availablePieces.m_availableRedPillars)
    {
        out << "Key: " << key << "Value: " << value << "\n";
    }
    for (const auto& [key, value] : availablePieces.m_availableRedBridges)
    {
        out << "Key: " << key << "Value: " << value << "\n";
    }
    for (const auto& [key, value] : availablePieces.m_availableBlackPillars)
    {
        out << "Key: " << key << "Value: " << value << "\n";
        for (const auto& [key, value] : availablePieces.m_availableBlackBridges)
        {
            out << "Key: " << key << "Value: " << value << "\n";
        }
    }
    return out << "\n\n";
}