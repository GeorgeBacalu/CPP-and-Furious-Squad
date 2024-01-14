#include "gtest/gtest.h"
#include "Pillar.h"

class PillarTest : public ::testing::Test
{
protected:
	Pillar pillar;

	void SetUp() override
	{
		pillar = { {3, 2}, Color::RED };
	}

	void TearDown() override
	{
		// ...
	}
};

TEST_F(PillarTest, DefaultConstructor)
{
	Pillar defaultPillar;
	EXPECT_EQ(defaultPillar.GetPosition().first, 0);
	EXPECT_EQ(defaultPillar.GetPosition().second, 0);
	EXPECT_EQ(defaultPillar.GetColor(), Color::NO_COLOR);
}

TEST_F(PillarTest, ColorConstructor)
{
	Pillar colorPillar{ Color::RED };
	EXPECT_EQ(colorPillar.GetPosition().first, 0);
	EXPECT_EQ(colorPillar.GetPosition().second, 0);
	EXPECT_EQ(colorPillar.GetColor(), Color::RED);
}

TEST_F(PillarTest, AllArgsContructor)
{
	EXPECT_EQ(pillar.GetPosition().first, 3);
	EXPECT_EQ(pillar.GetPosition().second, 2);
	EXPECT_EQ(pillar.GetColor(), Color::RED);
}

TEST_F(PillarTest, CopyConstructor)
{
	Pillar pillarCopy{ pillar };
	EXPECT_EQ(pillarCopy.GetPosition().first, 3);
	EXPECT_EQ(pillarCopy.GetPosition().second, 2);
	EXPECT_EQ(pillarCopy.GetColor(), Color::RED);
}

TEST_F(PillarTest, MoveConstructor)
{
	Pillar pillarMoved{ std::move(pillar) };
	EXPECT_EQ(pillarMoved.GetPosition().first, 3);
	EXPECT_EQ(pillarMoved.GetPosition().second, 2);
	EXPECT_EQ(pillarMoved.GetColor(), Color::RED);
	EXPECT_EQ(pillar.GetPosition().first, 0);
	EXPECT_EQ(pillar.GetPosition().second, 0);
	EXPECT_EQ(pillar.GetColor(), Color::NO_COLOR);
}

TEST_F(PillarTest, CopyingEquality)
{
	Pillar pillarCopy{ pillar };
	EXPECT_EQ(pillar, pillarCopy);
}

TEST_F(PillarTest, PositionSetter)
{
	Pillar pillar{ Position{ 3, 2 }, Color::RED };
	pillar.SetPosition(Position{ 4, 5 });
	EXPECT_EQ(pillar.GetPosition().first, 4);
	EXPECT_EQ(pillar.GetPosition().second, 5);
}

TEST_F(PillarTest, ColorSetter)
{
	Pillar pillar{ Position{ 3, 2 }, Color::RED };
	pillar.SetColor(Color::BLACK);
	EXPECT_EQ(pillar.GetColor(), Color::BLACK);
}

TEST_F(PillarTest, InvalidPositionSetter)
{
	Pillar pillar;
	EXPECT_THROW(pillar.SetPosition(Position{ 25, 25 }), std::out_of_range);
}