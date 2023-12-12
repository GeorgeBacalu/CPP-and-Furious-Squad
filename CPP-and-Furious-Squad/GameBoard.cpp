#include "GameBoard.h"
#include "ConsoleRenderer.h"
#include <fstream>
#include <queue>

int bridgeCount = 0;
GameBoard* GameBoard::instance = nullptr;
static uint16_t kAvailableRedPillars = 50;
static uint16_t kAvailableBlackPillars = 50;
static uint16_t kAvailableRedBridges = 50;
static uint16_t kAvailableBlackBridges = 50;

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

bool GameBoard::checkWin(Color playerColor)
{
	const auto& paths = playerColor == Color::RED ? s_paths.first : s_paths.second;
	for (const auto& path : paths)
	{
		auto [beginRow, beginColumn] = path.front().GetPosition();
		auto [endRow, endColumn] = path.back().GetPosition();
		if (playerColor == Color::RED && ((beginRow == 0 && endRow == kWidth - 1) || (beginRow == kWidth - 1 && endRow == 0)) ||
		   (playerColor == Color::BLACK && ((beginColumn == 0 && endColumn == kWidth - 1) || (beginColumn == kWidth - 1 && endColumn == 0))))
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

std::vector<Pillar> GameBoard::getRedPillars()
{
	s_redPillars.clear();
	for (const auto& pillar : s_pillars)
	{
		if (pillar.GetColor() == Color::RED)
			s_redPillars.push_back(pillar);
	}
	return s_redPillars;
}

std::vector<Pillar> GameBoard::getBlackPillars()
{
	s_blackPillars.clear();
	for (const auto& pillar : s_pillars)
	{
		if (pillar.GetColor() == Color::BLACK)
			s_blackPillars.push_back(pillar);
	}
	return s_blackPillars;
}

bool GameBoard::getPlayerTurn()
{
	return playerTurn;
}

void GameBoard::switchPlayerTurn()
{
	playerTurn = !playerTurn;
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

	// Check if the new pillar is placed on a board corner
	if ((newRow == 0 || newRow == kHeight - 1) && (newColumn == 0 || newColumn == kWidth - 1)) 
	{
		throw std::invalid_argument("Can't place pillar on any board corner!");
	}

	const std::vector<std::pair<int16_t, int16_t>> bridgeAllowedOffsets{ {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} };

	Color playerColor = (playerTurn) ? Color::RED : Color::BLACK;
	const std::string errorMessage = (playerTurn) ? "Red player can't place pillar on first or last column!" : "Black player can't place pillar on first or last row!";

	ProcessPlayerMove(newPillarPosition, playerColor, errorMessage, bridgeAllowedOffsets, newPillar);

	// Decrease available pillars
	if (playerTurn) {
		--kAvailableRedPillars;
	}
	else {
		--kAvailableBlackPillars;
	}

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

	std::vector<Pillar> playerPillars = (playerColor == Color::RED) ? getRedPillars() : getBlackPillars();

	std::vector<Bridge> newBridges;
	for (auto& playerPillar : playerPillars) {
		// evaluate if current pillar can form a bridge with the one that is being added
		const auto& [currentRow, currentColumn] = playerPillar.GetPosition();
		for (const auto& [offsetX, offsetY] : bridgeAllowedOffsets) {
			if (currentRow + offsetX == newRow && currentColumn + offsetY == newColumn) {
				if (CheckNoIntersections()) {
					newBridges.emplace_back(playerPillar, newPillar);
					break;
				}
			}
		}
	}

	if (newBridges.size() > 0)
	{
		auto& availableBridges = (playerColor == Color::RED) ? kAvailableRedBridges : kAvailableBlackBridges;
		if (availableBridges - newBridges.size() < 0)
		{
			std::vector<Bridge> chosenBridges;
			if (playerColor == Color::RED)
				chosenBridges = ConsoleRenderer::PlaceBridgesFromOptions(newBridges, availableBridges);
			else
				chosenBridges = ConsoleRenderer::PlaceRandomBridgesFromOptions(newBridges, availableBridges);
			availableBridges = 0;
			for (const auto& bridge : chosenBridges)
			{
				s_bridges.push_back(bridge);
			}
		}
	}
}

bool GameBoard::CheckNoIntersections()
{
	if (s_bridges.empty())
		return true;

	Bridge newBridge = s_bridges.back();

	for (const auto& existingBridge : s_bridges)
	{
		if (Intersects(existingBridge, newBridge))
			return false;
	}

	return true;
}

bool GameBoard::Intersects(const Bridge& bridge1, const Bridge& bridge2)
{
	const auto& start1 = bridge1.GetStartPillar().GetPosition();
	const auto& end1 = bridge1.GetEndPillar().GetPosition();
	const auto& start2 = bridge2.GetStartPillar().GetPosition();
	const auto& end2 = bridge2.GetEndPillar().GetPosition();

	return (start1 == start2 || start1 == end2 || end1 == start2 || end1 == end2) &&
		IntersectsOnSameAxis(bridge1, bridge2);
}

bool GameBoard::IntersectsOnSameAxis(const Bridge& bridge1, const Bridge& bridge2)
{
	const auto& start1 = bridge1.GetStartPillar().GetPosition();
	const auto& end1 = bridge1.GetEndPillar().GetPosition();
	const auto& start2 = bridge2.GetStartPillar().GetPosition();
	const auto& end2 = bridge2.GetEndPillar().GetPosition();

	if (start1.first == end1.first)
	{
		return start2.first == end2.first &&
			start1.first == start2.first &&
			IntersectsOnAxis(start1.second, end1.second, start2.second, end2.second);
	}
	else if (start1.second == end1.second)
	{
		return start2.second == end2.second &&
			start1.second == start2.second &&
			IntersectsOnAxis(start1.first, end1.first, start2.first, end2.first);
	}

	return false;
}

bool GameBoard::IntersectsOnAxis(int start1, int end1, int start2, int end2)
{
	return (start1 < end1) ? (start1 < start2 && start2 < end1) : (start1 > start2 && start2 > end1);
}

bool GameBoard::PlayerTurn()
{
	return playerTurn;
}

uint16_t GameBoard::GetAvailablePieces(IPiece* pieceType, Color color)
{
	if (dynamic_cast<Pillar*>(pieceType))
	{
		return color == Color::RED ? kAvailableRedPillars : kAvailableBlackPillars;
	}
	else {
		return color == Color::RED ? kAvailableRedBridges : kAvailableBlackBridges;
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