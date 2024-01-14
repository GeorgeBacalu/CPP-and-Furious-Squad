#include "gtest/gtest.h"
#include "ScoreBoard.h"

class ScoreBoardTest : public ::testing::Test
{
protected:
	ScoreBoard& scoreBoard = ScoreBoard::GetInstance();
};

TEST_F(ScoreBoardTest, GetInstance)
{
	ScoreBoard& newScoreBoard = ScoreBoard::GetInstance();
	EXPECT_EQ(&scoreBoard, &newScoreBoard);
}

TEST_F(ScoreBoardTest, DefaultScore)
{
	const Score& defaultScore = scoreBoard.GetScore();
	EXPECT_EQ(defaultScore.first, 0);
	EXPECT_EQ(defaultScore.second, 0);
}

TEST_F(ScoreBoardTest, SetScoreValid)
{
	scoreBoard.SetScore(10, 20);
	const Score& setScore = scoreBoard.GetScore();
	EXPECT_EQ(setScore.first, 10);
	EXPECT_EQ(setScore.second, 20);
}

TEST_F(ScoreBoardTest, SetScoreInvalidRedScore)
{
	EXPECT_THROW(scoreBoard.SetScore(ScoreBoard::kMaxScore + 1, 0), std::out_of_range);
}

TEST_F(ScoreBoardTest, SetScoreInvalidBlackScore)
{
	EXPECT_THROW(scoreBoard.SetScore(0, ScoreBoard::kMaxScore + 1), std::out_of_range);
}

TEST_F(ScoreBoardTest, UpdateScoreRed)
{
	scoreBoard.SetScore(0, 0);
	scoreBoard.UpdateScore(Color::RED);
	const Score& updatedScore = scoreBoard.GetScore();
	EXPECT_EQ(updatedScore.first, 1);
	EXPECT_EQ(updatedScore.second, 0);
}

TEST_F(ScoreBoardTest, UpdateScoreBlack)
{
	scoreBoard.SetScore(0, 0);
	scoreBoard.UpdateScore(Color::BLACK);
	const Score& updatedScore = scoreBoard.GetScore();
	EXPECT_EQ(updatedScore.first, 0);
	EXPECT_EQ(updatedScore.second, 1);
}

TEST_F(ScoreBoardTest, ResetScore)
{
	scoreBoard.UpdateScore(Color::RED);
	scoreBoard.UpdateScore(Color::BLACK);
	scoreBoard.ResetScore();
	const Score& resetScore = scoreBoard.GetScore();
	EXPECT_EQ(resetScore.first, 0);
	EXPECT_EQ(resetScore.second, 0);
}