#pragma once
#include"Pillar.h"
#include"Bridge.h"
#include"Utils.h"
#include<iostream>
#include<vector>
#include<memory>	
#include<optional>
#include<array>
class GameBoard
{
public:
	static const size_t kWidth{ 5 };
	static const size_t kHeight{ 5 };
private:
	static bool playerTurn;
	static bool invalid;
	std::array<std::array<std::optional<Pillar>, kWidth>, kHeight> s_matrix;
	std::vector <Bridge>s_bridges;
	std::vector<std::vector<Pillar>>ListaAdiacenta;
	std::pair<std::vector<std::vector<Pillar>>, std::vector<std::vector<Pillar>>>s_paths;
	std::vector<Pillar> s_pillars;
	std::vector<Pillar> s_redPillars;
	std::vector<Pillar> s_blackPillars;
	std::vector<Pillar>endingPillars;
	static GameBoard* instance;
	GameBoard();
	void PlacePillar(const Pillar& pillar);
public:
	GameBoard(const GameBoard& obj) = delete;
	static GameBoard* getInstance();
	~GameBoard() = default;

	//getR/setR
	uint16_t getSize();
	std::array<std::array<std::optional<Pillar>, kWidth>, kHeight>getMatrix();
	std::vector<Bridge>getBridges();
	void setSize(uint16_t size);
	void setMatrix(std::vector<std::vector<std::optional<Pillar>>>matrix);
	void setBridges(std::vector<Bridge> bridges);
	void setInvalid(bool invalid);
	std::vector<std::vector<Pillar>> getListaAdiacenta();
	std::pair<std::vector<std::vector<Pillar>>, std::vector<std::vector<Pillar>>>getPaths();
	std::vector<Pillar>getEndingPillars();
	std::vector<Pillar> getPillars();
	std::vector<Pillar> getRedPillars();
	std::vector<Pillar> getBlackPillars();

	//logic methods
	void PlacePillar(uint16_t row, uint16_t column);
	void ProcessNextMove(Pillar& newPillar);
	void ProcessPlayerMove(const Position& newPillarPosition, Color playerColor, const std::string& errorMessage, const std::vector<std::pair<int16_t, int16_t>>& bridgeAllowedOffsets, Pillar& newPillar);
	bool CheckNoIntersections();
	bool PlayerTurn();
	uint16_t GetAvailablePieces(IPiece* pieceType, Color color);

	void RemovePillar(uint16_t row, uint16_t column);
	bool IsFreeFoundation(uint16_t row, uint16_t column);
	void ResetGame();
	void SaveGame();
	void LoadGame();
	void ListaAdiacentaInit();
	void ListaAdiacentaUpdate();
	void bfs(const Pillar& start);
	bool checkWin(Color playerColor);
	void EndingPillarsInit();

	// related to AI player
	int64_t GetHashWithPosition(const Position& position) const;

	// overloaded operator
	std::optional<Pillar>& operator[](const Position& position);
	const std::optional<Pillar>& operator[](const Position& position) const;

	friend std::ostream& operator<<(std::ostream& out, const GameBoard& gb)
	{
		for (uint16_t i = 0; i < gb.kWidth; i++)
		{
			for (uint16_t j = 0; j < gb.kHeight; j++)
			{
				if (i == 0 && j == 0 || i == 0 && j == kHeight - 1 || i == kWidth - 1 && j == 0 || i == kWidth - 1 && j == kHeight - 1)
				{
					out << "   ";
				}
				else
				{
					if (!gb.s_matrix[i][j].has_value())
						out << 0 << "  ";
					else
					{
						if (static_cast<int>(gb.s_matrix[i][j].value().GetColor()) == 0)
							out << 1 << "  ";
						else
							out << 2 << "  ";
					}
				}
			}
			out << "\n\n";
		}
		return out;
	}
};


