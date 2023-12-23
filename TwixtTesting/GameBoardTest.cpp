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

// Add more test cases for other methods as needed

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}