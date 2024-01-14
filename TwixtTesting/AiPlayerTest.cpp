#include "gtest/gtest.h"
#include "AiPlayer.h"
#include "GameBoard.h"

class AiPlayerTest : public ::testing::Test
{
protected:
	GameBoard* gameBoard;
	AiPlayer aiPlayer{ *gameBoard, "Test AI Player Policy", Color::RED };

	void SetUp() override
	{
		gameBoard = GameBoard::GetInstance();
	}

	void TearDown() override
	{
		delete gameBoard;
	}
};

TEST_F(AiPlayerTest, AiPlayerConstructor)
{
	EXPECT_EQ(aiPlayer.GetName(), "Test AI Player Policy");
}

TEST_F(AiPlayerTest, LoadPolicyExistingPolicy)
{
	aiPlayer.LoadPolicy();
	EXPECT_FALSE(aiPlayer.GetStateActionCosts().empty());
}

TEST_F(AiPlayerTest, LoadPolicyNonExstingPolicy)
{
	AiPlayer noPolicyAiPlayer{ *gameBoard, "Non Existing Policy", Color::RED };
	noPolicyAiPlayer.LoadPolicy();
	EXPECT_TRUE(aiPlayer.GetStateActionCosts().empty());
}