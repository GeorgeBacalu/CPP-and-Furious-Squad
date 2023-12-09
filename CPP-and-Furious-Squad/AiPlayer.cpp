#include "AiPlayer.h"
#include <fstream>
#include <random>
#include <limits>

std::random_device randomDevice;
std::mt19937 randomEngine(randomDevice());

const float initialEstimation{ 0.5f };
const float explorationRate{ 0.1f };
const float learningRate{ 0.1f };

AiPlayer::AiPlayer(GameBoard& gameBoard, std::string_view policyName) : m_gameBoard{ gameBoard }, m_policyName{ policyName }
{
	LoadPolicy();
}

AiPlayer::~AiPlayer()
{
	SavePolicy();
}

Position AiPlayer::GetNextAction()
{
    std::vector<Position> possibleActions = GenerateActions(m_gameBoard);
    std::reference_wrapper<Position> bestAction{ possibleActions[0] };
    int64_t bestStateActionHash = 0;
    std::bernoulli_distribution bernoulliDistribution(explorationRate); // generates "true" with explorationRate probability
    if (bernoulliDistribution(randomEngine)) 
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
        estimation += learningRate * (target - estimation);
        target = estimation;
    }
    m_previousStateActions.clear();
}

std::vector<Position> AiPlayer::GenerateActions(const GameBoard& gameBoard) {
    std::vector<Position> possibleActions;
    for (uint32_t i = 0; i < GameBoard::kWidth; ++i)
        for (uint32_t j = 0; j < GameBoard::kHeight; ++j) {
            Position position = { i, j };
            if (!gameBoard[position])
                possibleActions.emplace_back(position);
        }
    return possibleActions;
}

void AiPlayer::SavePolicy() const {
    std::ofstream out(m_policyName);
    for (const auto& [stateActionHash, costEstimation] : m_stateActionCosts)
        if (stateActionHash != initialEstimation)
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