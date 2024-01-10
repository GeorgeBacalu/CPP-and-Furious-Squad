#include "gtest/gtest.h"
#include "GameBoard.h"

// Fixture for testing the GameBoard class
class GameBoardTest : public ::testing::Test {
protected:
    GameBoard* gameBoard;

    void SetUp() override {
        gameBoard = GameBoard::GetInstance();
    }

    void TearDown() override {
        delete gameBoard;
    }
};

// Test case for the PlacePillar method
TEST_F(GameBoardTest, PlacePillar) {
    uint16_t row = 1;
    uint16_t column = 2;
    ASSERT_NO_THROW(gameBoard->PlacePillar(row, column));
    ASSERT_TRUE(gameBoard->GetMatrix()[row][column].has_value());
}

// Test case for the SwitchPlayerTurn method
TEST_F(GameBoardTest, SwitchPlayerTurn) {
    bool initialTurn = gameBoard->GetPlayerTurn();
    gameBoard->SwitchPlayerTurn();
    ASSERT_NE(initialTurn, gameBoard->GetPlayerTurn());
}

// Test case for the ProcessNextMove method
TEST_F(GameBoardTest, ProcessNextMove) {
    Pillar newPillar{ {1, 2}, Color::RED };
    ASSERT_NO_THROW(gameBoard->ProcessNextMove(newPillar));
    // Add more assertions as needed
}

// Test case for the ValidateNewPillarPlacement method
TEST_F(GameBoardTest, ValidateNewPillarPlacement) {
    Pillar newPillar{ {1, 0}, Color::BLACK };
    ASSERT_THROW(gameBoard->ValidateNewPillarPlacement(newPillar, Color::BLACK), std::invalid_argument);
}

// Test case for the ProcessBridgesForNewPillar method
TEST_F(GameBoardTest, ProcessBridgesForNewPillar) {
    Pillar newPillar{ {2, 3}, Color::RED };
    std::vector<Bridge> newBridges = gameBoard->ProcessBridgesForNewPillar(newPillar);
    ASSERT_EQ(newBridges.size(), 2);  // Adjust based on the actual expected result
}

// Test case for the UpdateAvailablePieces method
TEST_F(GameBoardTest, UpdateAvailablePieces) {
    Pillar newPillar{ {3, 1}, Color::BLACK };
    std::vector<Bridge> newBridges;
    ASSERT_NO_THROW(gameBoard->UpdateAvailablePieces(newBridges, newPillar));
    ASSERT_EQ(gameBoard->GetAvailablePieces(nullptr, Color::BLACK), 49);  // Adjust based on the actual expected result
}

//Test case for the IsFreeFoundation method
TEST_F(GameBoardTest, IsFreeFoundation) {
    uint16_t row = 2;
    uint16_t column = 4;
    ASSERT_TRUE(gameBoard->IsFreeFoundation(row, column));
}

//Test case for the InitEndPillars method
TEST_F(GameBoardTest, InitEndPillars) {
    ASSERT_NO_THROW(gameBoard->InitEndPillars());
    ASSERT_EQ(gameBoard->GetEndPillars().size(), 4);  // Adjust based on the actual expected result
}

//Test case for the BFS method
TEST_F(GameBoardTest, BFS) {
    Pillar startPillar{ {0, 0}, Color::RED };
    ASSERT_NO_THROW(gameBoard->BFS(startPillar));
    // Add assertions based on the expected behavior of BFS
}

//Test case for the CheckWin method
TEST_F(GameBoardTest, CheckWin) {
    Color playerColor = Color::BLACK;
    ASSERT_FALSE(gameBoard->CheckWin(playerColor));
    // Add assertions based on the expected behavior of CheckWin
}

//Test case for the LoadGame method
TEST_F(GameBoardTest, LoadGame) {
    ASSERT_NO_THROW(gameBoard->LoadGame());
    // Add assertions based on the expected behavior after loading a game
}

//Test case for the SaveGame method
TEST_F(GameBoardTest, SaveGame) {
    ASSERT_NO_THROW(gameBoard->SaveGame());
    // Add assertions based on the expected behavior after saving a game
}

//Test case for the ResetGame method
TEST_F(GameBoardTest, ResetGame) {
    ASSERT_NO_THROW(gameBoard->ResetGame());
    // Add assertions based on the expected behavior after resetting the game
}

//Test case for the RemovePillar method
TEST_F(GameBoardTest, RemovePillar) {
    uint16_t row = 2;
    uint16_t column = 3;
    ASSERT_NO_THROW(gameBoard->RemovePillar(row, column));
    ASSERT_FALSE(gameBoard->GetMatrix()[row][column].has_value());
}

//Test case for the Intersects method
TEST_F(GameBoardTest, Intersects) {
    Bridge bridge1{ {1, 2}, {3, 2}, Color::RED };
    Bridge bridge2{ {2, 3}, {2, 1}, Color::BLACK };
    ASSERT_TRUE(gameBoard->Intersects(bridge1, bridge2));
}

//Test case for the CheckNoIntersections method
TEST_F(GameBoardTest, CheckNoIntersections) {
    ASSERT_TRUE(gameBoard->CheckNoIntersections());
}

//Test case for the IsPositionValid method
TEST_F(GameBoardTest, IsPositionValid) {
    Position invalidPosition{ 5, 5 };
    Position validPosition{ 2, 3 };
    ASSERT_FALSE(gameBoard->IsPositionValid(invalidPosition));
    ASSERT_TRUE(gameBoard->IsPositionValid(validPosition));
}

//Test case for the GetHashWithPosition method
TEST_F(GameBoardTest, GetHashWithPosition) {
    Position position{ 2, 3 };
    int64_t hash = gameBoard->GetHashWithPosition(position);
    ASSERT_NE(hash, 0);  // Adjust based on the actual expected result
}

//Test case for the InitAdjacencyList method
TEST_F(GameBoardTest, InitAdjacencyList) {
    ASSERT_NO_THROW(gameBoard->InitAdjacencyList());
    ASSERT_GT(gameBoard->GetAdjacencyList().size(), 0);
}

//Test case for the UpdateAdjacencyList method
TEST_F(GameBoardTest, UpdateAdjacencyList) {
    ASSERT_NO_THROW(gameBoard->UpdateAdjacencyList());
    ASSERT_GT(gameBoard->GetAdjacencyList().size(), 0);
}

//Test case for the InitEndPillars method (second test)
TEST_F(GameBoardTest, InitEndPillarsSecondTest) {
    gameBoard->ResetGame();
    ASSERT_NO_THROW(gameBoard->InitEndPillars());
    ASSERT_EQ(gameBoard->GetEndPillars().size(), 4);  // Adjust based on the actual expected result
}

//Test case for the PlacePillarQT method
TEST_F(GameBoardTest, PlacePillarQT) {
    uint16_t row = 2;
    uint16_t column = 1;
    ASSERT_NO_THROW(gameBoard->PlacePillarQT(row, column));
    ASSERT_TRUE(gameBoard->GetMatrix()[row][column].has_value());
}

//Test case for the ProcessNextMoveQT method
TEST_F(GameBoardTest, ProcessNextMoveQT) {
    Pillar newPillar{ {0, 1}, Color::BLACK };
    ASSERT_NO_THROW(gameBoard->ProcessNextMoveQT(newPillar));
    // Add more assertions as needed
}

//Test case for the LoadPillarsFromFile method
TEST_F(GameBoardTest, LoadPillarsFromFile) {
    std::string filename = "test_pillars.txt";
    ASSERT_NO_THROW(gameBoard->LoadPillarsFromFile(filename));
    // Add assertions based on the expected behavior after loading pillars from a file
}

//Test case for the LoadBridgesFromFile method
TEST_F(GameBoardTest, LoadBridgesFromFile) {
    std::string filename = "test_bridges.txt";
    ASSERT_NO_THROW(gameBoard->LoadBridgesFromFile(filename));
    // Add assertions based on the expected behavior after loading bridges from a file
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}