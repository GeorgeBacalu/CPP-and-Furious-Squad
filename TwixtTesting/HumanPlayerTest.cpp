#include "HumanPlayer.h"
#include <gtest/gtest.h>
#include <sstream>

class HumanPlayerTest : public ::testing::Test {
protected:
    std::streambuf* originalCin;
    std::istringstream input;
    std::streambuf* originalCout;
    std::ostringstream output;

    void SetUp() override {
        // Redirect cin and cout for testing GetNextAction
        originalCin = std::cin.rdbuf();
        originalCout = std::cout.rdbuf();
        std::cin.rdbuf(input.rdbuf());
        std::cout.rdbuf(output.rdbuf());
    }

    void TearDown() override {
        // Restore original cin and cout
        std::cin.rdbuf(originalCin);
        std::cout.rdbuf(originalCout);
    }
};

TEST_F(HumanPlayerTest, ConstructorSetsNameCorrectly) {
    HumanPlayer player("John Doe");
    EXPECT_EQ(player.GetName(), "John Doe");
}

TEST_F(HumanPlayerTest, GetNextActionReturnsCorrectPosition) {
    HumanPlayer player("John Doe");
    input.str("1 2\n"); // Simulate user input
    EXPECT_EQ(player.GetNextAction(), Position{ 1, 2 });
}

TEST_F(HumanPlayerTest, GetNextActionThrowsForInvalidInput) {
    HumanPlayer player("John Doe");
    input.str("999 999\n"); // Simulate invalid user input
    EXPECT_THROW(player.GetNextAction(), std::invalid_argument);
}

TEST_F(HumanPlayerTest, GetNameReturnsCorrectName) {
    HumanPlayer player("John Doe");
    EXPECT_EQ(player.GetName(), "John Doe");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}