#pragma once
#include<iostream>
#include"Pillar.h"

class GameBoard
{	
	static uint16_t s_size;
	static std::vector<std::vector<std::shared_ptr<Pillar>>> s_matrix;
public:
	//Constructor/Deconstructor/Rule of 3
	GameBoard();
	GameBoard(const uint16_t &size, const std::vector<std::vector<std::shared_ptr<Pillar>>>&matrix);
	GameBoard(const GameBoard &gb);
	~GameBoard();
	//getR/setR
	uint8_t getSize();
	std::vector<std::vector<std::shared_ptr<Pillar>>> getMatrix();
	void setSize(uint16_t size);
	void setMatrix(std::vector<std::vector<std::shared_ptr<Pillar>>> matrix);
	//logic methods
	void PlacePillar(uint16_t row , uint16_t column);
	void RemoveePillar(uint16_t row, uint16_t column);
	bool IsFreeFoundation(uint16_t row, uint16_t column);
	void ResetGame();

};


