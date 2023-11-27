#include "GameBoard.h"
#include<fstream>
#include<queue>

int bridgeCount = 0;

uint16_t GameBoard::s_size = BOARD_SIZE;
bool GameBoard::playerTurn = false;
bool GameBoard::invalid = false;
GameBoard* GameBoard::instance = nullptr;

GameBoard::GameBoard()
	:s_matrix{ std::vector<std::vector<std::optional<Pillar>>>(s_size, std::vector<std::optional<Pillar>>(s_size,std::nullopt)) }
	, s_bridges{ std::vector<Bridge>() }, ListaAdiacenta{ std::vector<std::vector<Pillar>>(s_size * s_size) }, 
	s_paths{ std::make_pair(std::vector<std::vector<Pillar>>(), std::vector<std::vector<Pillar>>()) }, endingPillars{ std::vector<Pillar>() }
{
}

void GameBoard::ListaAdiacentaInit()
{
	//generate ListaAdiacenta from Bridges
	//this->ListaAdiacenta = std::vector<std::vector<Pillar>>(s_size * s_size);
	for (auto it : s_bridges)
	{
		++bridgeCount;
		this->ListaAdiacenta[it.GetStartPillar().GetPosition().first * s_size + it.GetStartPillar().GetPosition().second].push_back(it.GetEndPillar());
		this->ListaAdiacenta[it.GetEndPillar().GetPosition().first * s_size + it.GetEndPillar().GetPosition().second].push_back(it.GetStartPillar());
	}
}

void GameBoard::ListaAdiacentaUpdate()
{
	//update ListaAdiacenta for the last added bridge
	//this->ListaAdiacenta = std::vector<std::vector<Pillar>>(s_size * s_size);
	if (bridgeCount < s_bridges.size())
	{
		auto it = s_bridges.back();
		++bridgeCount;
		this->ListaAdiacenta[it.GetStartPillar().GetPosition().first * s_size + it.GetStartPillar().GetPosition().second].push_back(it.GetEndPillar());
		this->ListaAdiacenta[it.GetEndPillar().GetPosition().first * s_size + it.GetEndPillar().GetPosition().second].push_back(it.GetStartPillar());
	}
	else
	{
		//throw std::invalid_argument("No bridge was added");
	}
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

	ListaAdiacentaUpdate();
	/*red_paths = s_paths.first;
	black_paths = s_paths.second;*/

	if (start.GetColor() == Color::RED)
		red_q.push(start);
	else
		black_q.push(start);

	//the red_paths must have it's first element the start pillar
	while (!red_q.empty())
	{
		Pillar current = red_q.front();
		red_q.pop();
		red_path.push_back(current);
		visited[current.GetPosition().first * s_size + current.GetPosition().second] = true;
		if (current != start and std::find(endingPillars.begin(), endingPillars.end(), current) != endingPillars.end())
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
		if (current!=start and std::find(endingPillars.begin(), endingPillars.end(), current) != endingPillars.end())
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

	//print paths
	/*std::cout << "RED PATHS:\n";
	for (auto it : s_paths.first)
	{
		for (auto it2 : it)
			std::cout << it2 << ' ';
		std::cout << "\n";
	}
	std::cout << "BLACK PATHS:\n";
	for (auto it : s_paths.second)
	{
		for (auto it2 : it)
			std::cout << it2 << ' ';
		std::cout << "\n";
	}*/
	//system("pause");
}
//
bool GameBoard::redWin()
{
	//check if red won. To win, red must have a path from top to bottom
	for (auto it : s_paths.first)
	{
		auto begin = *it.begin();
		auto end = *(it.end() - 1);
		if (begin != end and std::find(endingPillars.begin(), endingPillars.end(), begin) != endingPillars.end() && std::find(endingPillars.begin(), endingPillars.end(), end) != endingPillars.end())
			return true;
	}
	return false;
}

bool GameBoard::blackWin()
{
	//check if black won
	for (auto it : s_paths.second)
	{
		auto begin = *it.begin();
		auto end = *(it.end() - 1);
		if (begin != end and std::find(endingPillars.begin(), endingPillars.end(), begin) != endingPillars.end() && std::find(endingPillars.begin(), endingPillars.end(), end) != endingPillars.end())
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

void GameBoard::setInvalid(bool invalid)
{
	this->invalid = invalid;
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
		if (playerTurn)
			P.SetColor(Color::RED);
		else
			P.SetColor(Color::BLACK);
		ProcessNextMove(P);

		if (invalid == false)
		{
			s_matrix[row][column] = std::optional<Pillar>{ P };
			s_pillars.push_back(P);
		}
	}

	else
	{
		throw std::invalid_argument("Position is not valid");
	}
	
	invalid = false;
}
void GameBoard::PlacePillar(const Pillar& pillar)
{
	Position position{ pillar.GetPosition() };
	if (IsFreeFoundation(position.first, position.second))
	{
		s_matrix[position.first][position.second] = std::optional<Pillar>{ pillar };
		s_pillars.push_back(pillar);
	}
	else
		throw std::invalid_argument("Position is not valid");
}

void GameBoard::ProcessNextMove(const Pillar& newPillar) {
	const Position& newPillarPosition = newPillar.GetPosition();
	const auto& [newRow, newColumn] = newPillarPosition;
	const std::vector<std::pair<int16_t, int16_t>> bridgeAllowedOffsets{ {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} };

	// exclude corners
	if ((newRow == 0 && newColumn == 0) ||
		(newRow == 0 && newColumn == BOARD_SIZE - 1) ||
		(newRow == BOARD_SIZE - 1 && newColumn == 0) ||
		(newRow == BOARD_SIZE - 1 && newColumn == BOARD_SIZE - 1))
		throw std::invalid_argument("Can't place pillar on any board corner!");
	if (playerTurn) // playerTurn = true <=> red's turn
		ProcessPlayerMove(newPillarPosition, Color::RED, "Red player can't place pillar on first or last column!", bridgeAllowedOffsets, newPillar);
	else
		ProcessPlayerMove(newPillarPosition, Color::BLACK, "Black player can't place pillar on first or last row!", bridgeAllowedOffsets, newPillar);
	playerTurn = !playerTurn;
}

void GameBoard::ProcessPlayerMove(const Position& newPillarPosition, Color playerColor, const std::string& errorMessage, const std::vector<std::pair<int16_t, int16_t>>& bridgeAllowedOffsets, const Pillar& newPillar) {
	const auto& [newRow, newColumn] = newPillarPosition;

	// exclude first and last columns or rows
	if ((playerColor == Color::RED && (newColumn == 0 || newColumn == BOARD_SIZE - 1)) ||
		(playerColor == Color::BLACK && (newRow == 0 || newRow == BOARD_SIZE - 1)))
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

	/*Pillar newPillar = Pillar(newPillarPosition, playerColor);
	s_pillars.push_back(newPillar);
	s_matrix[newRow][newColumn] = newPillar;*/

	for (const auto& playerPillar : playerPillars)
	{
		// evaluate if current pillar can form a bridge with the one that is being added
		const auto& [currentRow, currentColumn] = playerPillar.GetPosition();
		for (const auto& [offsetX, offsetY] : bridgeAllowedOffsets)
		{
			if (currentRow + offsetX == newRow && currentColumn + offsetY == newColumn)
			{
				if (CheckNoIntersections())
				{
					Bridge newBridge = Bridge(playerPillar, newPillar);
					s_bridges.push_back(newBridge);
					break;
				}
			}
		}
	}
}

bool GameBoard::CheckNoIntersections()
{
	return true;
}

bool GameBoard::PlayerTurn()
{
	return playerTurn;
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
	/*std::cout << "Are you sure you want to reset the game?\n";
	std::cout << "1 for yes , 0 for no";
	bool ok;
	std::cin >> ok;*/
	if (true)
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
		try
		{
			fp >> p;
			PlacePillar(p);
		}
		catch (const std::invalid_argument& e)
		{
			std::cerr << e.what() << std::endl;
			break;
		}
	}
	fp.close();
	std::ifstream fb("bridges.prodb");
	uint16_t redCount{ 0 }, blackCount{ 0 };
	while (!fb.eof())
	{
		Bridge b;
		try
		{
			fb >> b;
			s_bridges.push_back(b);
			if (b.GetEndPillar().GetColor() == Color::RED)
				++redCount;
			else
				++blackCount;
		}
		catch (const std::invalid_argument& e)
		{
			std::cerr << e.what() << std::endl;
			break;
		}
	}
	if (redCount > blackCount)
		playerTurn = false;//black turn
	else
		playerTurn = true;//red turn
	fb.close();
}

int64_t GameBoard::GetHashWithPosition(const Position& position) const {
	// TODO: implement hashing functionality
	return 0;
}

std::optional<Pillar>& GameBoard::operator[](const Position& position)
{
	const auto& [row, column] = position;
	if (row < 0 || row > BOARD_SIZE || column < 0 || column > BOARD_SIZE)
	{
		throw std::out_of_range("Position out of bounds");
	}
	return s_matrix[row][column];
}

const std::optional<Pillar>& GameBoard::operator[](const Position& position) const
{
	const auto& [row, column] = position;
	if (row < 0 || row > BOARD_SIZE || column < 0 || column > BOARD_SIZE)
	{
		throw std::out_of_range("Position out of bounds");
	}
	return s_matrix[row][column];
}
