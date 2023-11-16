#include "GameBoard.h"
#include<fstream>
#include<queue>

uint16_t GameBoard::s_size = 0;
std::vector<std::vector<std::optional<Pillar>>>GameBoard::s_matrix;
std::vector<Bridge>GameBoard::s_bridges;
std::vector<std::vector<Pillar>>GameBoard::ListaAdiacenta;
std::pair<std::vector<std::vector<Pillar>>, std::vector<std::vector<Pillar>>>GameBoard::s_paths;
std::vector<Pillar> GameBoard::s_pillars;
std::vector<Pillar>GameBoard::endingPillars;
bool GameBoard::playerTurn = false;
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
	if (start.GetColor() == Color::RED)
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
		for (auto it2 : it)
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
		for (auto it2 : it)
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

std::vector<Pillar> GameBoard::getPillars()
{
	return s_pillars;
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
	else
		throw std::invalid_argument("Position is not valid");
}
void GameBoard::PlacePillar(const Pillar& pillar)
{
	Position position{ pillar.GetPosition() };
	if (IsFreeFoundation(position.first, position.second))
		s_matrix[position.first][position.second] = std::optional<Pillar>{ pillar };
	else
		throw std::invalid_argument("Position is not valid");
}

void GameBoard::ProcessNextMove(const Position& newPillarPosition) {
	const auto& [newRow, newColumn] = newPillarPosition;
	const std::vector<std::pair<int16_t, int16_t>> bridgeAllowedOffsets{ {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} };

	// exclude corners
	if ((newRow == 0 && newColumn == 0) ||
		(newRow == 0 && newColumn == BOARD_SIZE - 1) ||
		(newRow == BOARD_SIZE - 1 && newColumn == 0) ||
		(newRow == BOARD_SIZE - 1 && newColumn == BOARD_SIZE - 1))
		throw std::invalid_argument("Can't place pillar on any board corner!");
	playerTurn = !playerTurn;
	if (playerTurn) // playerTurn = true <=> red's turn
		ProcessPlayerMove(newPillarPosition, Color::RED, "Red player can't place pillar on first or last column!", bridgeAllowedOffsets);
	else
		ProcessPlayerMove(newPillarPosition, Color::BLACK, "Black player can't place pillar on first or last row!", bridgeAllowedOffsets);
}

void GameBoard::ProcessPlayerMove(const Position& newPillarPosition, Color playerColor, const std::string& errorMessage, const std::vector<std::pair<int16_t, int16_t>>& bridgeAllowedOffsets) {
	const auto& [newRow, newColumn] = newPillarPosition;

	// exclude first and last columns or rows
	if ((playerColor == Color::RED && (newColumn == 0 || newColumn == BOARD_SIZE - 2)) ||
		(playerColor == Color::BLACK && (newRow == 0 || newRow == BOARD_SIZE - 2)))
	{
		throw std::invalid_argument(errorMessage);
	}

	// filter pillars
	std::vector<Pillar> playerPillars;
	for (const auto& pillar : s_pillars)
	{
		if (pillar.GetColor() == playerColor)
			playerPillars.push_back(pillar);
	}

	Pillar newPillar = Pillar(newPillarPosition, playerColor);
	s_pillars.push_back(newPillar);
	s_matrix[newRow][newColumn] = newPillar;

	for (const auto& playerPillar : playerPillars)
	{
		// evaluate if current pillar can form a bridge with the one that is being added
		const auto& [currentRow, currentColumn] = playerPillar.GetPosition();
		for (const auto& [offsetX, offsetY] : bridgeAllowedOffsets)
		{
			if (currentRow + offsetX == newRow && currentColumn + offsetY == newColumn)
			{
				Bridge newBridge = Bridge(playerPillar, newPillar);
				s_bridges.push_back(newBridge);
				break;
			}
		}
	}
}

void GameBoard::RemovePillar(uint16_t row, uint16_t column)
{
	if (!IsFreeFoundation(row, column))
	{
		s_matrix[row][column] = std::optional<Pillar>{};
		for (std::vector<Bridge>::iterator it = s_bridges.begin(); it != s_bridges.end();)
		{
			if (it->GetEndPillar().GetPosition() == Position{ row,column })
				s_bridges.erase(it);
			else
				if (it->GetStartPillar().GetPosition() == Position{ row,column })
				{
					s_bridges.erase(it);
				}
				else ++it;
		}
	}
	else
		throw std::invalid_argument("There is no pillar to erase");
}

bool GameBoard::IsFreeFoundation(uint16_t row, uint16_t column)
{
	if (row == 0 && column == 0)
		return false;
	if (row == 0 && column == 23)
		return false;
	if (row == 23 && column == 0)
		return false;
	if (row == 23 && column == 23)
		return false;
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
	GameBoard::s_bridges = std::vector<Bridge>();
	GameBoard::ListaAdiacenta = std::vector<std::vector<Pillar>>(s_size * s_size);
	GameBoard::s_paths = std::make_pair(std::vector<std::vector<Pillar>>(), std::vector<std::vector<Pillar>>());
	GameBoard::endingPillars = std::vector<Pillar>();
	GameBoard::s_pillars = std::vector<Pillar>();
}

void GameBoard::SaveGame()
{
	std::ofstream f("pillars.prodb");
	for (auto row : s_matrix)
	{
		for (auto column : row)
		{
			if (column.has_value())
				f << column.value();
		}

	}
	f.close();
	std::ofstream f1("bridges.prodb");
	for (auto it : s_bridges)
		f1 << it;
	f1.close();

}

void GameBoard::LoadGame()
{
	ResetGame();
	playerTurn = true;
	std::ifstream fp("pillars.prodb");
	while (!fp.eof())
	{
		Pillar p;
		fp >> p;
		PlacePillar(p);
	}
	fp.close();
	std::ifstream fb("bridges.prodb");
	uint16_t redCount{ 0 }, blackCount{ 0 };
	while (!fb.eof())
	{
		Bridge b;
		if (fb >> b)
		{
			s_bridges.push_back(b);
			if (b.GetEndPillar().GetColor() == Color::RED)
				++redCount;
			else
				++blackCount;
		}
	}
	if (redCount > blackCount)
		playerTurn = true;
	else
		playerTurn = false;
	fb.close();
}
