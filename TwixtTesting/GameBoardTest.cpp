#include "gtest/gtest.h"
#include "GameBoard.h"

class GameBoardTest : public ::testing::Test
{
protected:

    GameBoard* gameBoard;

	void SetUp() override
	{
		gameBoard = GameBoard::GetInstance();
		gameBoard->SetPlayerTurn(true);
		gameBoard->SetMatrix(std::array<std::array<std::optional<Pillar>, GameBoard::kWidth>, GameBoard::kHeight>{});
	}

	void TearDown() override
	{
		delete gameBoard;
		gameBoard = nullptr;
	}
};

TEST_F(GameBoardTest, GetWidth)
{
	EXPECT_EQ(gameBoard->GetWidth(), GameBoard::kWidth);
}

TEST_F(GameBoardTest, GetHeight)
{
	EXPECT_EQ(gameBoard->GetHeight(), GameBoard::kHeight);
}

TEST_F(GameBoardTest, GetPlayerTurn)
{
	EXPECT_TRUE(gameBoard->GetPlayerTurn());
}

TEST_F(GameBoardTest, GetInvalid)
{
	EXPECT_FALSE(gameBoard->GetInvalid());
}

TEST_F(GameBoardTest, GetMatrix)
{
	EXPECT_EQ(gameBoard->GetMatrix().size(), GameBoard::kHeight);
	EXPECT_EQ(gameBoard->GetMatrix()[0].size(), GameBoard::kWidth);
}

TEST_F(GameBoardTest, SetPlayerTurn)
{
	gameBoard->SetPlayerTurn(false);
	EXPECT_FALSE(gameBoard->GetPlayerTurn());
}

TEST_F(GameBoardTest, SetInvalid)
{
	gameBoard->SetInvalid(true);
	EXPECT_TRUE(gameBoard->GetInvalid());
}

TEST_F(GameBoardTest, PlacePillarValidPosition)
{
	gameBoard->PlacePillar(2, 2);
	//EXPECT_EQ(gameBoard->GetMatrix()[2][2].value(), Pillar{ {2, 2}, Color::RED });
}

TEST_F(GameBoardTest, IsPositionValid)
{
    Position invalidPosition{ -1, -1 };
    Position validPosition{ 2, 3 };
    ASSERT_FALSE(gameBoard->IsPositionValid(invalidPosition));
    ASSERT_TRUE(gameBoard->IsPositionValid(validPosition));
}

TEST_F(GameBoardTest, PlacePillar) 
{
    uint16_t row = 1;
    uint16_t column = 2;
    ASSERT_NO_THROW(gameBoard->PlacePillar(row, column));
    ASSERT_TRUE(gameBoard->GetMatrix()[row][column].has_value());
}

TEST_F(GameBoardTest, SwitchPlayerTurn) 
{
    bool initialTurn = gameBoard->GetPlayerTurn();
    gameBoard->SwitchPlayerTurn();
    ASSERT_NE(initialTurn, gameBoard->GetPlayerTurn());
}

TEST_F(GameBoardTest, ProcessNextMove) 
{
    Pillar newPillar{ {1, 2}, Color::RED };
    ASSERT_NO_THROW(gameBoard->ProcessNextMove(newPillar));
}

TEST_F(GameBoardTest, ValidateNewPillarPlacement) 
{
    Pillar newPillar{ {1, 2}, Color::BLACK };
    ASSERT_NO_THROW(gameBoard->ValidateNewPillarPlacement(newPillar, Color::BLACK));
}

TEST_F(GameBoardTest, ProcessBridgesForNewPillar) 
{
    Pillar newPillar{ {2, 3}, Color::RED };
    std::vector<Bridge> newBridges = gameBoard->ProcessBridgesForNewPillar(newPillar);
    ASSERT_EQ(newBridges.size(), 0);
}

TEST_F(GameBoardTest, IsFreeFoundation) 
{
    uint16_t row = 2;
    uint16_t column = 4;
    ASSERT_TRUE(gameBoard->IsFreeFoundation(row, column));
}

TEST_F(GameBoardTest, InitEndPillars) 
{
    ASSERT_NO_THROW(gameBoard->InitEndPillars());
    ASSERT_EQ(gameBoard->GetEndPillars().size(), 0);
}

TEST_F(GameBoardTest, BFS) 
{
    Pillar startPillar{ {0, 0}, Color::RED };
    ASSERT_NO_THROW(gameBoard->BFS(startPillar));
}

TEST_F(GameBoardTest, CheckWin) 
{
    Color playerColor = Color::BLACK;
    ASSERT_FALSE(gameBoard->CheckWin(playerColor));
}

TEST_F(GameBoardTest, LoadGame) 
{
    ASSERT_NO_THROW(gameBoard->LoadGame());
}

TEST_F(GameBoardTest, SaveGame) 
{
    ASSERT_NO_THROW(gameBoard->SaveGame());
}

TEST_F(GameBoardTest, ResetGame) 
{
    ASSERT_NO_THROW(gameBoard->ResetGame()); 
}

TEST_F(GameBoardTest, RemovePillar) 
{
    uint16_t row = 2;
    uint16_t column = 3;
    ASSERT_THROW(gameBoard->RemovePillar(row, column), std::invalid_argument);
    ASSERT_FALSE(gameBoard->GetMatrix()[row][column].has_value());
}

TEST_F(GameBoardTest, GetHashWithPosition) 
{
    Position position{ 2, 3 };
    int64_t hash = gameBoard->GetHashWithPosition(position);
    ASSERT_NE(hash, 0);
}

TEST_F(GameBoardTest, InitAdjacencyList) 
{
    ASSERT_NO_THROW(gameBoard->InitAdjacencyList());
    ASSERT_GT(gameBoard->GetAdjacencyList().size(), 0);
}

TEST_F(GameBoardTest, UpdateAdjacencyList) 
{
    ASSERT_NO_THROW(gameBoard->UpdateAdjacencyList());
    ASSERT_GT(gameBoard->GetAdjacencyList().size(), 0);
}

TEST_F(GameBoardTest, IsFreeFoundationCorner) 
{
    ASSERT_FALSE(gameBoard->IsFreeFoundation(0, 0));
    gameBoard->PlacePillar(2, 3);
    ASSERT_FALSE(gameBoard->IsFreeFoundation(2, 3));
}