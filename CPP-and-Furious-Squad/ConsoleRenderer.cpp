#include "ConsoleRenderer.h"
#include <iomanip>

void ConsoleRenderer::Render(GameBoard* gb)
{
    system("CLS");
    std::cout << "  ";
    for (int i = 1; i < GameBoard::kWidth; ++i)
    {
        PrintCell(gb->getMatrix()[0][i]);
    }
    std::cout << "\n  " << std::setfill('-') << std::setw(GameBoard::kWidth) << "\n";

    for (int i = 1; i < GameBoard::kWidth; ++i)
    {
        PrintCell(gb->getMatrix()[i][0], true);

        for (int j = 1; j < GameBoard::kHeight; ++j)
        {
            PrintCell(gb->getMatrix()[i][j]);
        }

        PrintCell(gb->getMatrix()[i][GameBoard::kHeight - 1], true, true);
    }

    std::cout << "  " << std::setfill('-') << std::setw(GameBoard::kWidth) << "\n";
    for (int i = 0; i < GameBoard::kWidth; ++i)
    {
        PrintCell(gb->getMatrix()[GameBoard::kWidth - 1][i]);
    }
    std::cout << "\nDimension: " << GameBoard::kWidth << "\n";
}

void ConsoleRenderer::PrintCell(const std::optional<Pillar>& cell, bool withBorder, bool endOfLine)
{
    if (cell.has_value())
    {
        char cellChar = (cell.value().GetColor() == Color::RED) ? 'R' : 'B';
        std::cout << cellChar;
    }
    else
    {
        std::cout << ".";
    }

    if (withBorder)
    {
        std::cout << "|";
    }

    if (endOfLine)
    {
        std::cout << "\n";
    }
}

void ConsoleRenderer::TakeInput(GameBoard* gb)
{
    if (gb->PlayerTurn())
        std::cout << "Red player's turn\n";
    else
        std::cout << "Black player's turn\n";
    std::cout << "Enter the coordinates of the pillar you want to place: ";
    int x, y;
    std::cin >> x >> y;
    //place pillar and switch player
    try
    {
        gb->PlacePillar(x, y);
        gb->EndingPillarsInit();
        for (auto it : gb->getEndingPillars())
            gb->bfs(it);
    }
    catch (std::invalid_argument& exception)
    {
        std::cerr << exception.what() << "\n";
    }
}