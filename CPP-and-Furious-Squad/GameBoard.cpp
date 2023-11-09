#include "GameBoard.h"
#include<fstream>
#include<queue>

uint16_t GameBoard::s_size = 0;
std::vector<std::vector<std::optional<Pillar>>>GameBoard::s_matrix;
std::vector<Bridge>GameBoard::s_bridges;
std::vector<std::vector<Pillar>>GameBoard::ListaAdiacenta;
std::pair<std::vector<std::vector<Pillar>>, std::vector<std::vector<Pillar>>>GameBoard::s_paths;
std::vector<Pillar>GameBoard::endingPillars;
GameBoard* GameBoard::instance = NULL;

GameBoard::GameBoard()
{
	GameBoard::s_size = 24;
	for (uint16_t i = 0; i < s_size; ++i)
	{
		std::vector<std::optional<Pillar>> row;
		for (uint16_t j = 0; j < s_size; ++j)
		{
			row.push_back(std::optional<Pillar>{});
		}
		GameBoard::s_matrix.push_back(row);
	}
	GameBoard::s_bridges = std::vector<Bridge>();
	GameBoard::ListaAdiacenta = std::vector<std::vector<Pillar>>(s_size * s_size);
	GameBoard::s_paths = std::make_pair(std::vector<std::vector<Pillar>>(), std::vector<std::vector<Pillar>>());
	GameBoard::endingPillars = std::vector<Pillar>();
}

void GameBoard::ListaAdiacentaInit()
{
	//generate ListaAdiacenta from Bridges
	this->ListaAdiacenta = std::vector<std::vector<Pillar>>(s_size * s_size);
	for (auto it : s_bridges)
	{
		this->ListaAdiacenta[it.GetStartPillar().GetPosition().first * s_size + it.GetStartPillar().GetPosition().second].push_back(it.GetEndPillar());
		this->ListaAdiacenta[it.GetEndPillar().GetPosition().first * s_size + it.GetEndPillar().GetPosition().second].push_back(it.GetStartPillar());
	}
}

void GameBoard::ListaAdiacentaUpdate()
{

}

void GameBoard::bfs(const Pillar& start)
{
	//make paths from start to endingPillars. All the paths must have same color of pillars
	std::vector<std::vector<Pillar>> red_paths;
	std::vector<std::vector<Pillar>> black_paths;
	std::vector<Pillar> red_path;
	std::vector<Pillar> black_path;
	std::vector<bool> visited(s_size * s_size, false);
	std::queue<Pillar> red_q;
	std::queue<Pillar> black_q;
	if(start.GetColor()==Color::RED)
		red_q.push(start);
	else
		black_q.push(start);
	while (!red_q.empty())
	{
		Pillar current = red_q.front();
		red_q.pop();
		red_path.push_back(current);
		visited[current.GetPosition().first * s_size + current.GetPosition().second] = true;
		if (std::find(endingPillars.begin(), endingPillars.end(), current) != endingPillars.end())
		{
			red_paths.push_back(red_path);
			red_path.clear();
		}
		for (auto it : ListaAdiacenta[current.GetPosition().first * s_size + current.GetPosition().second])
		{
			if (!visited[it.GetPosition().first * s_size + it.GetPosition().second] && it.GetColor() == Color::RED)
				red_q.push(it);
		}
	}
	while (!black_q.empty())
	{
		Pillar current = black_q.front();
		black_q.pop();
		black_path.push_back(current);
		visited[current.GetPosition().first * s_size + current.GetPosition().second] = true;
		if (std::find(endingPillars.begin(), endingPillars.end(), current) != endingPillars.end())
		{
			black_paths.push_back(black_path);
			black_path.clear();
		}
		for (auto it : ListaAdiacenta[current.GetPosition().first * s_size + current.GetPosition().second])
		{
			if (!visited[it.GetPosition().first * s_size + it.GetPosition().second] && it.GetColor() == Color::BLACK)
				black_q.push(it);
		}
	}
	s_paths = std::make_pair(red_paths, black_paths);
}
//
bool GameBoard::redWin()
{
	//check if red won
	for (auto it : s_paths.first)
	{
		for(auto it2:it)
			if (std::find(endingPillars.begin(), endingPillars.end(), it2) != endingPillars.end())
				return true;
	}
	return false;
}

bool GameBoard::blackWin()
{
	//check if black won
	for (auto it : s_paths.second)
	{
		for(auto it2:it)
			if (std::find(endingPillars.begin(), endingPillars.end(), it2) != endingPillars.end())
				return true;
	}
	return false;
}

void GameBoard::EndingPillarsInit()
{
	//generate endingPillars from s_matrix
	for (uint16_t i = 0; i < s_size; ++i)
	{
		for (uint16_t j = 0; j < s_size; ++j)
		{
			if (s_matrix[i][j].has_value())
			{
				if (i == 0 || i == s_size - 1 || j == 0 || j == s_size - 1)
					endingPillars.push_back(s_matrix[i][j].value());
			}
		}
	}
}


GameBoard* GameBoard::getInstance()
{
	if (instance == NULL)
	{
		instance = new GameBoard();
		return instance;
	}
	else
		return instance;
}
uint16_t GameBoard::getSize()
{
	return s_size;
}

std::vector<std::vector<std::optional<Pillar>>> GameBoard::getMatrix()
{
	return s_matrix;
}

std::vector<Bridge> GameBoard::getBridges()
{
	return s_bridges;
}

void GameBoard::setSize(uint16_t size)
{
	s_size = size;
}

void GameBoard::setMatrix(std::vector<std::vector<std::optional<Pillar>>> matrix)
{
	s_matrix = std::vector<std::vector<std::optional<Pillar>>>();
	for (uint16_t i = 0; i < s_size; ++i)
	{
		std::vector<std::optional<Pillar>> row;
		for (uint16_t j = 0; j < s_size; ++j)
		{
			std::optional<Pillar> aux{ matrix[i][j] };
			row.push_back(aux);
		}
		s_matrix.push_back(row);
	}
}
void GameBoard::setBridges(std::vector<Bridge> bridges)
{
	s_bridges = bridges;
}

std::vector<std::vector<Pillar>> GameBoard::getListaAdiacenta()
{
	return ListaAdiacenta;
}

std::pair<std::vector<std::vector<Pillar>>, std::vector<std::vector<Pillar>>> GameBoard::getPaths()
{
	return s_paths;
}

std::vector<Pillar> GameBoard::getEndingPillars()
{
	return endingPillars;
}

void GameBoard::PlacePillar(uint16_t row, uint16_t column)
{
	if (IsFreeFoundation(row, column))
	{
		Pillar P;
		P.SetPosition(std::make_pair(row, column));
		s_matrix[row][column] = std::optional<Pillar>{ P };
		//the changes to color and rules for bridges are to be implemented later 
	}
}

void GameBoard::RemovePillar(uint16_t row, uint16_t column)
{
	if (!IsFreeFoundation(row, column))
		s_matrix[row][column] = std::optional<Pillar>{};
}

bool GameBoard::IsFreeFoundation(uint16_t row, uint16_t column)
{
	return !s_matrix[row][column].has_value();
}

void GameBoard::ResetGame()
{
	std::cout << "Are you sure you want to reset the game?\n";
	std::cout << "1 for yes , 0 for no";
	bool ok;
	std::cin >> ok;
	if (ok == 1)
	{
		for (uint16_t i = 0; i < s_size; ++i)
		{
			for (uint16_t j = 0; j < s_size; ++j)
				s_matrix[i][j] = std::optional<Pillar>{};
		}
	}
}

void GameBoard::SaveGame()
{
	std::ofstream f("pillar.out");
	for (auto row : s_matrix)
	{
		for (auto column : row)
		{
			if (column.has_value())
				f << column.value();
		}
				
	}
	f.close();
	std::ofstream f1("bridges.out");
	for (auto it : s_bridges)
		f1 << it;
	f1.close();
}
void GameBoard::LoadGame()
{

}
