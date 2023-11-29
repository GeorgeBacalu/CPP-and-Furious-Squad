#include "GameBoard.h"
#include<fstream>
#include<queue>

int bridgeCount = 0;
bool GameBoard::playerTurn = true;
bool GameBoard::invalid = false;
GameBoard* GameBoard::instance = nullptr;

GameBoard::GameBoard()
	:s_bridges{ std::vector<Bridge>() }, ListaAdiacenta{ std::vector<std::vector<Pillar>>(kWidth * kHeight) }, 
	s_paths{ std::make_pair(std::vector<std::vector<Pillar>>(), std::vector<std::vector<Pillar>>()) }, endingPillars{ std::vector<Pillar>() }
{
}

void GameBoard::ListaAdiacentaInit()
{
	//generate ListaAdiacenta from Bridges
	//this->ListaAdiacenta = std::vector<std::vector<Pillar>>(kWidth * kWidth);
	for (auto it : s_bridges)
	{
		++bridgeCount;
		this->ListaAdiacenta[it.GetStartPillar().GetPosition().first * kWidth + it.GetStartPillar().GetPosition().second].push_back(it.GetEndPillar());
		this->ListaAdiacenta[it.GetEndPillar().GetPosition().first * kHeight + it.GetEndPillar().GetPosition().second].push_back(it.GetStartPillar());
	}
}

void GameBoard::ListaAdiacentaUpdate()
{
	//update ListaAdiacenta for the last added bridge
	//this->ListaAdiacenta = std::vector<std::vector<Pillar>>(kWidth * kWidth);
	if (bridgeCount < s_bridges.size())
	{
		auto it = s_bridges.back();
		++bridgeCount;
		this->ListaAdiacenta[it.GetStartPillar().GetPosition().first * kWidth + it.GetStartPillar().GetPosition().second].push_back(it.GetEndPillar());
		this->ListaAdiacenta[it.GetEndPillar().GetPosition().first * kHeight + it.GetEndPillar().GetPosition().second].push_back(it.GetStartPillar());
	}
	else
	{
		//throw std::invalid_argument("No bridge was added");
	}
}

void GameBoard::bfs(const Pillar& start)
{
	//make paths from start to endingPillars. All the paths must have same color of pillars
	std::vector<std::vector<Pillar>> paths;
	std::vector<Pillar> path;
	std::vector<bool> visited(kWidth * kHeight, false);
	std::queue<Pillar> q;

	ListaAdiacentaUpdate();
	/*red_paths = s_paths.first;
	black_paths = s_paths.second;*/

	q.push(start);

	//the red_paths must have it's first element the start pillar
	while (!q.empty())
	{
		Pillar current = q.front();
		q.pop();
		path.push_back(current);
		visited[current.GetPosition().first * kWidth + current.GetPosition().second] = true;
		if (current != start and (current.GetPosition().first == 0 or current.GetPosition().first == kWidth - 1))
		{
			paths.push_back(path);
			path.clear();
		}
		for (auto it : ListaAdiacenta[current.GetPosition().first * kWidth + current.GetPosition().second])
		{
			if (!visited[it.GetPosition().first * kWidth + it.GetPosition().second] && it.GetColor() == start.GetColor())
				q.push(it);
		}
	}

	/*while (!black_q.empty())
	{
		Pillar current = black_q.front();
		black_q.pop();
		black_path.push_back(current);
		visited[current.GetPosition().first * kWidth + current.GetPosition().second] = true;
		if (current!=start and std::find(endingPillars.begin(), endingPillars.end(), current) != endingPillars.end())
		{
			black_paths.push_back(black_path);
			black_path.clear();
		}
		for (auto it : ListaAdiacenta[current.GetPosition().first * kWidth + current.GetPosition().second])
		{
			if (!visited[it.GetPosition().first * kWidth + it.GetPosition().second] && it.GetColor() == Color::BLACK)
				black_q.push(it);
		}
	}*/

	if (start.GetColor() == Color::RED)
		s_paths.first = paths;
	else
		s_paths.second = paths;

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
		if (begin != end and ((begin.GetPosition().first == 0 and end.GetPosition().first == kWidth - 1) or (begin.GetPosition().first == kWidth - 1 and end.GetPosition().first == 0)))
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
		if (begin != end and ((begin.GetPosition().second == 0 && end.GetPosition().second == kWidth - 1) or (begin.GetPosition().second == kWidth - 1 && end.GetPosition().second == 0)))
			return true;
	}
	return false;
}

void GameBoard::EndingPillarsInit()
{
	//generate endingPillars from s_matrix
	for (uint16_t i = 0; i < kWidth; ++i)
	{
		for (uint16_t j = 0; j < kHeight; ++j)
		{
			if (s_matrix[i][j].has_value())
			{
				if (i == 0 || i == kWidth - 1 || j == 0 || j == kWidth - 1)
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
	return kWidth;
}

std::array<std::array<std::optional<Pillar>, GameBoard::kWidth>, GameBoard::kHeight> GameBoard::getMatrix()
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

void GameBoard::setMatrix(std::vector<std::vector<std::optional<Pillar>>> matrix)
{
	for (size_t i = 0; i < kWidth; ++i) {
		for (size_t j = 0; j < kHeight; ++j) {
			if (i < matrix.size() && j < matrix[i].size()) {
				s_matrix[i][j] = matrix[i][j];
			}
			else {
				s_matrix[i][j] = std::nullopt;
			}
		}
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

void GameBoard::ProcessNextMove(Pillar& newPillar) {
	const Position& newPillarPosition = newPillar.GetPosition();
	const auto& [newRow, newColumn] = newPillarPosition;
	const std::vector<std::pair<int16_t, int16_t>> bridgeAllowedOffsets{ {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} };

	// exclude corners
	if ((newRow == 0 && newColumn == 0) ||
		(newRow == 0 && newColumn == kWidth - 1) ||
		(newRow == kHeight - 1 && newColumn == 0) ||
		(newRow == kHeight - 1 && newColumn == kWidth - 1))
		throw std::invalid_argument("Can't place pillar on any board corner!");
	if (playerTurn) // playerTurn = true <=> red's turn
		ProcessPlayerMove(newPillarPosition, Color::RED, "Red player can't place pillar on first or last column!", bridgeAllowedOffsets, newPillar);
	else
		ProcessPlayerMove(newPillarPosition, Color::BLACK, "Black player can't place pillar on first or last row!", bridgeAllowedOffsets, newPillar);
	playerTurn = !playerTurn;
}

void GameBoard::ProcessPlayerMove(const Position& newPillarPosition, Color playerColor, const std::string& errorMessage, const std::vector<std::pair<int16_t, int16_t>>& bridgeAllowedOffsets, Pillar& newPillar) {
	const auto& [newRow, newColumn] = newPillarPosition;

	// exclude first and last columns or rows
	if ((playerColor == Color::RED && (newColumn == 0 || newColumn == kWidth - 1)) ||
		(playerColor == Color::BLACK && (newRow == 0 || newRow == kHeight - 1)))
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

	for (auto& playerPillar : playerPillars)
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
		for (uint16_t i = 0; i < kWidth; ++i)
		{
			for (uint16_t j = 0; j < kHeight; ++j)
				s_matrix[i][j] = std::optional<Pillar>{};
		}
	}
	GameBoard::s_bridges = std::vector<Bridge>();
	GameBoard::ListaAdiacenta = std::vector<std::vector<Pillar>>(kWidth * kHeight);
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
	if (row < 0 || row > kHeight || column < 0 || column > kWidth)
	{
		throw std::out_of_range("Position out of bounds");
	}
	return s_matrix[row][column];
}

const std::optional<Pillar>& GameBoard::operator[](const Position& position) const
{
	const auto& [row, column] = position;
	if (row < 0 || row > kHeight || column < 0 || column > kWidth)
	{
		throw std::out_of_range("Position out of bounds");
	}
	return s_matrix[row][column];
}