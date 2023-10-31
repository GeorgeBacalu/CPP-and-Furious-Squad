#pragma once
#include"Pillar.h"
#include<iostream>
#include<vector>
#include<memory>
#include<optional>
class GameBoard
{
	static uint16_t s_size;
	static std::vector<std::vector<std::optional<Pillar>>> s_matrix;
	static GameBoard* instance;
	GameBoard();
public:
	GameBoard(const GameBoard& obj) = delete;
	static GameBoard* getInstance();
	~GameBoard() = default;
	//getR/setR
	uint16_t getSize();
	std::vector<std::vector<std::optional<Pillar>>> getMatrix();
	void setSize(uint16_t size);
	void setMatrix(std::vector<std::vector<std::optional<Pillar>>>matrix);
	//logic methods
	void PlacePillar(uint16_t row, uint16_t column);
	void RemovePillar(uint16_t row, uint16_t column);
	bool IsFreeFoundation(uint16_t row, uint16_t column);
	void ResetGame();
};


