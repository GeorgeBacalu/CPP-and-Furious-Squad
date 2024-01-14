#include "gtest/gtest.h"
#include "Bridge.h"

class BridgeTest : public ::testing::Test {
protected:
	Bridge bridge;

	void SetUp() override {
		bridge = Bridge{ Pillar{1, 1, Color::RED}, Pillar{3, 2, Color::RED} };
	}
};

TEST_F(BridgeTest, Constructor)
{
	const auto& [startRow, startColumn] = bridge.GetStartPillar().GetPosition();
	const auto& [endRow, endColumn] = bridge.GetEndPillar().GetPosition();
	EXPECT_EQ(startRow, 1);
	EXPECT_EQ(startColumn, 1);
	EXPECT_EQ(endRow, 3);
	EXPECT_EQ(endColumn, 2);
	EXPECT_EQ(pillar1.GetColor(), Color::RED);
	EXPECT_EQ(pillar2.GetColor(), Color::RED);
}