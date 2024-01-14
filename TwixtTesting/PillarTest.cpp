#include "gtest/gtest.h"
#include "Pillar.h"

class PillarTest : public ::testing::Test {
protected:
	Pillar pillar;

	void SetUp() override {
		pillar = Pillar{ 3, 2, Color::RED };
	}
};

TEST_F(PillarTest, Constructor)
{
	const auto& [row, column] = pillar.GetPosition();
	EXPECT_EQ(row, 3);
	EXPECT_EQ(column, 2);
	EXPECT_EQ(pillar.GetColor(), Color::RED);
}

TEST_F(PillarTest, ColorTest)
{
	EXPECT_EQ(pillar.GetColor(), Color::RED);
	pillar.SetColor(Color::BLACK);
	EXPECT_EQ(pillar.GetColor(), Color::BLACK);
}

TEST_F(PillarTest, PositionTest)
{
	const auto& [row, column] = pillar.GetPosition();
	EXPECT_EQ(row, 3);
	EXPECT_EQ(column, 2);
	pillar.SetPosition(1, 1);
	const auto& [newRow, newColumn] = pillar.GetPosition();
	EXPECT_EQ(newRow, 1);
	EXPECT_EQ(newColumn, 1);
}