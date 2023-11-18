#pragma once
#include"Pillar.h"
#include"Bridge.h"
#include "ConsoleRenderer.h"
#include<iostream>
#include<vector>
#include<memory>	
#include<optional>
class GameBoard
{
	static uint16_t s_size;
	static bool playerTurn;
	static bool invalid;
	static std::vector<std::vector<std::optional<Pillar>>> s_matrix;
	static std::vector <Bridge>s_bridges;
	static std::vector<std::vector<Pillar>>ListaAdiacenta;
	static std::pair<std::vector<std::vector<Pillar>>, std::vector<std::vector<Pillar>>>s_paths;
	static std::vector<Pillar> s_pillars;
	static std::vector<Pillar>endingPillars;
	static GameBoard* instance;
	GameBoard();
	void PlacePillar(const Pillar& pillar);


public:
	GameBoard(const GameBoard& obj) = delete;
	static GameBoard* getInstance();
	~GameBoard() = default;

	//getR/setR
	uint16_t getSize();
	std::vector<std::vector<std::optional<Pillar>>> getMatrix();
	std::vector<Bridge>getBridges();
	void setSize(uint16_t size);
	void setMatrix(std::vector<std::vector<std::optional<Pillar>>>matrix);
	void setBridges(std::vector<Bridge> bridges);
	void setInvalid(bool invalid);
	std::vector<std::vector<Pillar>> getListaAdiacenta();
	std::pair<std::vector<std::vector<Pillar>>, std::vector<std::vector<Pillar>>>getPaths();
	std::vector<Pillar>getEndingPillars();
	std::vector<Pillar> getPillars();

	//logic methods
	void PlacePillar(uint16_t row, uint16_t column);
	void ProcessNextMove(const Pillar& newPillar);
	void ProcessPlayerMove(const Position& newPillarPosition, Color playerColor, const std::string& errorMessage, const std::vector<std::pair<int16_t, int16_t>>& bridgeAllowedOffsets, const Pillar& newPillar);
	bool CheckNoIntersections();

	void RemovePillar(uint16_t row, uint16_t column);
	bool IsFreeFoundation(uint16_t row, uint16_t column);
	void ResetGame();
	void SaveGame();
	void LoadGame();
	void ListaAdiacentaInit();
	void ListaAdiacentaUpdate();
	void bfs(const Pillar& start);
	bool redWin();
	bool blackWin();
	void EndingPillarsInit();

	friend std::ostream& operator<<(std::ostream& out, const GameBoard& gb)
	{
		for (uint16_t i = 0; i < gb.s_size; i++)
		{
			for (uint16_t j = 0; j < gb.s_size; j++)
			{
				if (i == 0 && j == 0 || i == 0 && j == BOARD_SIZE - 1 || i == BOARD_SIZE - 1 && j == 0 || i == BOARD_SIZE - 1 && j == BOARD_SIZE - 1)
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


