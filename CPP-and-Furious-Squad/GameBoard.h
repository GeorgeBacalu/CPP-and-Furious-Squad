#pragma once
#include"Pillar.h"
#include"Bridge.h"
#include<iostream>
#include<vector>
#include<memory>
#include<optional>
class GameBoard
{
	static uint16_t s_size;
	static std::vector<std::vector<std::optional<Pillar>>> s_matrix;
	static std::vector <Bridge>s_bridges;
	static std::vector<std::vector<Pillar>>ListaAdiacenta;
	static std::vector<std::vector<Pillar>>s_paths;
	static std::vector<Pillar>endingPillars;
	static GameBoard* instance;
	GameBoard();

	void ListaAdiacentaInit();
	void ListaAdiacentaUpdate();
	std::vector<std::vector<Pillar>> bfs(const Pillar& start, const std::vector<std::vector<Pillar>>& ListaAdiacenta, const Pillar& end);
	void EndingPillarsInit();

public:
	GameBoard(const GameBoard& obj) = delete;
	static GameBoard* getInstance();
	~GameBoard()=default;

	//getR/setR
	uint16_t getSize();
	std::vector<std::vector<std::optional<Pillar>>> getMatrix();
	std::vector<Bridge>getBridges();
	void setSize(uint16_t size);
	void setMatrix(std::vector<std::vector<std::optional<Pillar>>>matrix);
	void setBridges(std::vector<Bridge> bridges);

	//logic methods
	void PlacePillar(uint16_t row, uint16_t column);
	void RemovePillar(uint16_t row, uint16_t column);
	bool IsFreeFoundation(uint16_t row, uint16_t column);
	void ResetGame();
	void SaveGame();
	void LoadGame();
	friend std::ostream& operator<<(std::ostream& out, const GameBoard& gb)
	{
		for (uint16_t i = 0; i < gb.s_size; i++)
		{
			for (uint16_t j = 0; j < gb.s_size; j++)
			{
				if (!gb.s_matrix[i][j].has_value())
					out << 0 << " ";
				else
				{
					if (static_cast<int>(gb.s_matrix[i][j].value().GetColor()) == 0)
						out << 1 << " ";
					else
						out << 2 << " ";
				}
			}
			out << "\n";

		}
		return out;
	}
};


