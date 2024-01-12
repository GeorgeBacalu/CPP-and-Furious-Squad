#include "AiPlayer.h"
#include <fstream>
#include <random>
#include <limits>
#include <math.h>

std::random_device randomDevice;
std::mt19937 randomEngine(randomDevice());

const float initialEstimation{ 0.5f };
const float explorationRate{ 0.1f };
const float learningRate{ 0.1f };

AiPlayer::AiPlayer(GameBoard& gameBoard, std::string_view policyName, Color color) : m_gameBoard{ gameBoard }, m_policyName{ policyName }, m_color{ color }
{
	LoadPolicy();
}

AiPlayer::~AiPlayer()
{
	SavePolicy();
}

Position AiPlayer::GetNextAction()
{
    std::vector<Position> possibleActions = GenerateNonAdjacentActions(m_gameBoard);

    if (possibleActions.empty())
    {
        throw std::invalid_argument("TIE");
    }

    std::reference_wrapper<Position> bestAction{ possibleActions[0] };
    int64_t bestStateActionHash = 0;
    std::bernoulli_distribution bernoulliDistribution(explorationRate); // generates "true" with explorationRate probability
    if (bernoulliDistribution(randomEngine) < explorationRate)
    {
        // randomly pick an action
        std::uniform_int_distribution<uint32_t> uniformDistribution(0, (uint32_t)possibleActions.size() - 1);
        bestAction = possibleActions[uniformDistribution(randomEngine)];
        bestStateActionHash = m_gameBoard.GetHashWithPosition(bestAction);
        auto itMap = m_stateActionCosts.insert({ bestStateActionHash, initialEstimation });
    }
    else 
    {
        // compute best action
        float bestActionEstimation = -std::numeric_limits<float>::infinity();
        for (auto& action : possibleActions) 
        {
            int64_t stateActionHash = m_gameBoard.GetHashWithPosition(action);
            auto [iterator, inserted] = m_stateActionCosts.insert({ stateActionHash, initialEstimation });
            float currentActionEstimation = iterator->second;
            if (currentActionEstimation > bestActionEstimation) 
            {
                bestAction = action;
                bestStateActionHash = stateActionHash;
                bestActionEstimation = currentActionEstimation;
            }
        }
    }
    m_previousStateActions.emplace_back(bestStateActionHash);

    return bestAction;
}

Position AiPlayer::RandomAction()
{
    std::vector<Position> possibleActions = GenerateNonAdjacentActions(m_gameBoard);   

    if (possibleActions.empty())
        throw std::invalid_argument("TIE");

    Position action;

    std::uniform_int_distribution<uint32_t> uniformDistribution(0, (uint32_t)possibleActions.size() - 1);
    action = possibleActions[uniformDistribution(randomEngine)];
    return action;
}

std::string_view AiPlayer::GetName() const 
{ 
    return m_policyName;
}

void AiPlayer::FreeReward(float target)
{
    for (auto it = m_previousStateActions.rbegin(); it != m_previousStateActions.rend(); ++it)
    {
        const auto& stateAction = *it;
        float& estimation = m_stateActionCosts[stateAction];

        // Check if the current state is adjacent to the previous state
        //if (IsAdjacentState(stateAction, m_previousStateActions))
        //{
        //    // Penalize the AI for adjacent positions
        //    estimation -= learningRate;
        //}
        //else
        {
            // Update the estimation using the target
            estimation += learningRate * (target - estimation);
            target = estimation;
        }
    }
    m_previousStateActions.clear();
}

bool AiPlayer::IsAdjacentState(int64_t currentState, const std::vector<int64_t>& previousStates) const
{
    // Implement the logic to check if the current state is adjacent to any of the previous states
    // You may need to extract the positions from the hash values and compare them
    // For simplicity, let's assume that currentState and previousStates contain row/column information in the hash values

    // Extract positions from currentState
    Position currentPos = ExtractPositionFromHash(currentState);

    // Check adjacency with each previous state
    for (const auto& prevState : previousStates)
    {
        Position prevPos = ExtractPositionFromHash(prevState);

        // Check if the positions are adjacent (you may need to define the adjacency criteria)
        if (ArePositionsAdjacent(currentPos, prevPos))
        {
            return true;
        }
    }

    return false;
}

Position AiPlayer::ExtractPositionFromHash(int64_t stateHash) const
{
    int row, column;

    if (stateHash == 0)
    {
        row = 0;
        column = 0;
    }
    else if (stateHash == 1)
    {
        row = 0;
        column = GameBoard::kWidth - 1;
    }
    else if (stateHash == 2)
    {
        row = GameBoard::kHeight - 1;
        column = 0;
    }
    else if (stateHash == 3)
    {
        row = GameBoard::kHeight - 1;
        column = GameBoard::kWidth - 1;
    }
    else if (stateHash < 4 + GameBoard::kWidth - 2)
    {
        row = 0;
        column = 1 + stateHash - 4;
    }
    else if (stateHash < 4 + 2 * (GameBoard::kWidth - 2) + GameBoard::kHeight - 2)
    {
        row = 1 + stateHash - (4 + GameBoard::kWidth - 2);
        column = (stateHash % 2 == 0) ? 0 : GameBoard::kWidth - 1;
    }
    else
    {
        stateHash -= 4 + 2 * (GameBoard::kWidth - 2) + 2 * (GameBoard::kHeight - 2);

        row = 1 + (stateHash - 1) / (GameBoard::kWidth - 2);
        column = 1 + (stateHash - 1) % (GameBoard::kWidth - 2);
    }

    return { row, column };
}


bool AiPlayer::ArePositionsAdjacent(const Position& pos1, const Position& pos2) const
{
    // Check if the positions are adjacent in terms of right, left, up, or down
    // For simplicity, let's assume that the positions are adjacent if they are on the same row or column

    int row1 = static_cast<int>(pos1.first);
    int column1 = static_cast<int>(pos1.second);
    int row2 = static_cast<int>(pos2.first);
    int column2 = static_cast<int>(pos2.second);

    return (std::abs(row1 - row2) <= 1 && std::abs(column1 - column2) <= 1);

}


bool AiPlayer::isPositionValid(const Position& position,GameBoard& gameBoard)
{
	if(position.first < 0 || position.first >= GameBoard::kWidth || position.second < 0 || position.second >= GameBoard::kHeight)
		return false;
    //check if position is at the corners of the board
    if ((position.first == 0 && position.second == 0) || (position.first == 0 && position.second == GameBoard::kHeight - 1) ||
        		(position.first == GameBoard::kWidth - 1 && position.second == 0) || (position.first == GameBoard::kWidth - 1 && position.second == GameBoard::kHeight - 1))
		return false;

    //if AI is red, check if position is on first or last row
    if (m_color == Color::RED)
        if(position.second == 0 or position.second == GameBoard::kHeight - 1)
			return false;
        else
            return !gameBoard[position] and gameBoard.IsPositionValid(position);
    //if AI is black, check if position is on first or last column
    if (m_color == Color::BLACK)
   		if(position.first == 0 or position.first == GameBoard::kWidth - 1)
            return false;
        else
            return !gameBoard[position] and gameBoard.IsPositionValid(position);
}

std::vector<Position> AiPlayer::GenerateActions(GameBoard& gameBoard) {
    std::vector<Position> possibleActions;
    for (uint32_t i = 0; i < GameBoard::kWidth; ++i)
        for (uint32_t j = 0; j < GameBoard::kHeight; ++j) {
            Position position = { i, j };
            if (isPositionValid(position,gameBoard))
                possibleActions.emplace_back(position);
        }
    return possibleActions;
}

std::vector<Position> AiPlayer::GenerateNonAdjacentActions(GameBoard& gameBoard)
{
    std::vector<Position> allPossibleActions = GenerateActions(gameBoard);

    // Filter out actions that are adjacent to any of the previous actions
    std::vector<Position> nonAdjacentActions;
    for (const auto& action : allPossibleActions)
    {
        if (!IsAdjacentState(m_gameBoard.GetHashWithPosition(action), m_previousStateActions))
        {
			nonAdjacentActions.emplace_back(action);
		}
    }

    return nonAdjacentActions;
}

void AiPlayer::SavePolicy() const {
    std::ofstream out(m_policyName);
    for (const auto& [stateActionHash, costEstimation] : m_stateActionCosts)
            out << stateActionHash << " " << costEstimation << "\n";
}

void AiPlayer::LoadPolicy() {
    std::ifstream in(m_policyName);
    if (in.fail()) return;
    while (!in.eof()) {
        int64_t stateActionHash;
        float estimation;
        in >> stateActionHash >> estimation;
        m_stateActionCosts[stateActionHash] = estimation;
    }
}

void AiPlayer::SetPolicyName(std::string_view policyName)
{
	m_policyName = policyName;
}
