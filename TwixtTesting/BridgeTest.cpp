#include "gtest/gtest.h"
#include "Bridge.h"

class BridgeTest : public ::testing::Test 
{
protected:
	Bridge bridge;

	void SetUp() override {
		bridge = Bridge{ Pillar{{1, 1}, Color::RED}, Pillar{{3, 2}, Color::RED} };
	}
};

TEST_F(BridgeTest, DefaultConstructor)
{
	Bridge defaultBridge;
	EXPECT_EQ(defaultBridge.GetStartPillar().GetPosition().first, 0);
	EXPECT_EQ(defaultBridge.GetStartPillar().GetPosition().second, 0);
	EXPECT_EQ(defaultBridge.GetStartPillar().GetColor(), Color::NO_COLOR);
	EXPECT_EQ(defaultBridge.GetEndPillar().GetPosition().first, 0);
	EXPECT_EQ(defaultBridge.GetEndPillar().GetPosition().second, 0);
	EXPECT_EQ(defaultBridge.GetEndPillar().GetColor(), Color::NO_COLOR);
}

TEST_F(BridgeTest, AllArgsConstructor)
{
	EXPECT_EQ(bridge.GetStartPillar().GetPosition().first, 1);
	EXPECT_EQ(bridge.GetStartPillar().GetPosition().second, 1);
	EXPECT_EQ(bridge.GetStartPillar().GetColor(), Color::RED);
	EXPECT_EQ(bridge.GetEndPillar().GetPosition().first, 3);
	EXPECT_EQ(bridge.GetEndPillar().GetPosition().second, 2);
	EXPECT_EQ(bridge.GetEndPillar().GetColor(), Color::RED);
}

TEST_F(BridgeTest, CopyConstructor)
{
	Bridge bridgeCopy{ bridge };
	EXPECT_EQ(bridgeCopy.GetStartPillar().GetPosition().first, 1);
	EXPECT_EQ(bridgeCopy.GetStartPillar().GetPosition().second, 1);
	EXPECT_EQ(bridgeCopy.GetStartPillar().GetColor(), Color::RED);
	EXPECT_EQ(bridgeCopy.GetEndPillar().GetPosition().first, 3);
	EXPECT_EQ(bridgeCopy.GetEndPillar().GetPosition().second, 2);
	EXPECT_EQ(bridgeCopy.GetEndPillar().GetColor(), Color::RED);
}

TEST_F(BridgeTest, MoveConstructor)
{
	Bridge bridgeMoved{ std::move(bridge) };
	EXPECT_EQ(bridgeMoved.GetStartPillar().GetPosition().first, 1);
	EXPECT_EQ(bridgeMoved.GetStartPillar().GetPosition().second, 1);
	EXPECT_EQ(bridgeMoved.GetStartPillar().GetColor(), Color::RED);
	EXPECT_EQ(bridgeMoved.GetEndPillar().GetPosition().first, 3);
	EXPECT_EQ(bridgeMoved.GetEndPillar().GetPosition().second, 2);
	EXPECT_EQ(bridgeMoved.GetEndPillar().GetColor(), Color::RED);
	EXPECT_EQ(bridge.GetStartPillar().GetPosition().first, 0);
	EXPECT_EQ(bridge.GetStartPillar().GetPosition().second, 0);
	EXPECT_EQ(bridge.GetStartPillar().GetColor(), Color::NO_COLOR);
	EXPECT_EQ(bridge.GetEndPillar().GetPosition().first, 0);
	EXPECT_EQ(bridge.GetEndPillar().GetPosition().second, 0);
	EXPECT_EQ(bridge.GetEndPillar().GetColor(), Color::NO_COLOR);
}

TEST_F(BridgeTest, CopyingEquality)
{
	Bridge bridgeCopy{ bridge };
	EXPECT_EQ(bridgeCopy, bridge);
}

TEST_F(BridgeTest, GetStartPillar)
{
	EXPECT_EQ(bridge.GetStartPillar().GetPosition().first, 1);
	EXPECT_EQ(bridge.GetStartPillar().GetPosition().second, 1);
	EXPECT_EQ(bridge.GetStartPillar().GetColor(), Color::RED);
}

TEST_F(BridgeTest, GetEndPillar)
{
	EXPECT_EQ(bridge.GetEndPillar().GetPosition().first, 3);
	EXPECT_EQ(bridge.GetEndPillar().GetPosition().second, 2);
	EXPECT_EQ(bridge.GetEndPillar().GetColor(), Color::RED);
}

TEST_F(BridgeTest, IsValid)
{
	EXPECT_TRUE(bridge.IsValid());
}

TEST_F(BridgeTest, IsInvalid)
{
	Bridge invalidBridge{ Pillar{{1, 1}, Color::RED}, Pillar{{1, 1}, Color::RED} };
	EXPECT_FALSE(invalidBridge.IsValid());
}