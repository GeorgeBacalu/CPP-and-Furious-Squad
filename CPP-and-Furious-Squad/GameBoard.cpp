#include "GameBoard.h"
#include<fstream>
#include<queue>

uint16_t GameBoard::s_size = 0;
std::vector<std::vector<std::optional<Pillar>>>GameBoard::s_matrix;
std::vector<Bridge>GameBoard::s_bridges;
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
}

void GameBoard::ListaAdiacentaInit()
{
	//generate ListaAdiacenta from Bridges
	ListaAdiacenta = std::vector<std::vector<Pillar>>(s_size * s_size);
	for (auto it : s_bridges)
	{
		ListaAdiacenta[it.GetStartPillar().GetPosition().first * s_size + it.GetStartPillar().GetPosition().second].push_back(it.GetEndPillar());
		ListaAdiacenta[it.GetEndPillar().GetPosition().first * s_size + it.GetEndPillar().GetPosition().second].push_back(it.GetStartPillar());
	}
}

void GameBoard::ListaAdiacentaUpdate()
{

}

std::vector<std::vector<Pillar>> GameBoard::bfs(const Pillar& start, const std::vector<std::vector<Pillar>>& ListaAdiacenta, const Pillar& end)
{
	std::vector<std::vector<Pillar>> paths;
	std::vector<Pillar> path;
	std::vector<bool> visited(s_size * s_size, false);
	std::queue<Pillar> q;
	q.push(start);
	visited[start.GetPosition().first * s_size + start.GetPosition().second] = true;
	//the paths must have same color
	while (!q.empty())
	{
		Pillar node = q.front();
		q.pop();
		path.push_back(node);
		if (node == end)
		{
			paths.push_back(path);
			path.pop_back();
		}
		for (auto it : ListaAdiacenta[node.GetPosition().first * s_size + node.GetPosition().second])
		{
			if (!visited[it.GetPosition().first * s_size + it.GetPosition().second])
			{
				visited[it.GetPosition().first * s_size + it.GetPosition().second] = true;
				q.push(it);
			}
		}
	}

	return paths;
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
	std::ofstream f("bridges.out");
	for (auto it : s_bridges)
		f << it;
	f.close();
}
void GameBoard::LoadGame()
{

}
