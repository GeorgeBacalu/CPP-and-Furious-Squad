#include "HumanPlayer.h"
#include <gtest/gtest.h>
#include <sstream>

class HumanPlayerTest : public ::testing::Test 
{
protected:
    HumanPlayer humanPlayer{ "Test Player" };
    std::streambuf* initialCin;
    std::istringstream input;
    std::streambuf* initialCout;
    std::ostringstream output;

    void SetUp() override {
        // Redirect cin and cout for testing GetNextAction
        initialCin = std::cin.rdbuf();
        initialCout = std::cout.rdbuf();
        std::cin.rdbuf(input.rdbuf());
        std::cout.rdbuf(output.rdbuf());
    }

    void TearDown() override {
        // Restore original cin and cout
        std::cin.rdbuf(initialCin);
        std::cout.rdbuf(initialCout);
    }
};

TEST_F(HumanPlayerTest, GetNextActionValid) {
    input.str("3 4\n");
    EXPECT_EQ(humanPlayer.GetNextAction().first, 3);
    EXPECT_EQ(humanPlayer.GetNextAction().second, 4);
}

TEST_F(HumanPlayerTest, GetNextActionThrowsForInvalidInput) {
    input.str("999 999\n");
    EXPECT_THROW(humanPlayer.GetNextAction(), std::invalid_argument);
}

TEST_F(HumanPlayerTest, GetName)
{
    EXPECT_EQ(humanPlayer.GetName(), "Test Player");
}