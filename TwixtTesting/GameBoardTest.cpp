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


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}