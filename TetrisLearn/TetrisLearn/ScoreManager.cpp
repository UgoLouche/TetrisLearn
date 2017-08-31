#include "ScoreManager.hpp"

#include "const.hpp"

#include "IBoard.hpp"
#include "Settings.hpp"

ScoreManager::ScoreManager(IBoard & parent) : parent(parent)
{
}

void ScoreManager::addScore_custom(size_t lineCleared)
{
	setScore(score + simulateScore(lineCleared, level));

	// Check for levelUp
	levelProgress += lineCleared;
	while (levelProgress >= Settings::get().linesToLevelup)
	{
		setLevel(level + 1);
		levelProgress -= (size_t)Settings::get().linesToLevelup;
	}

}

size_t ScoreManager::getScore_custom() const
{
	return score;
}

size_t ScoreManager::getLevel_custom() const
{
	return level;
}

void ScoreManager::setScore(size_t newScore)
{
	score = newScore;
	parent.changeScoreUI(score);
}

void ScoreManager::setLevel(size_t newLevel)
{
	level = newLevel;
	parent.changeLevelUI(level);
}

size_t ScoreManager::simulateScore_custom(size_t lineCleared, size_t level) const
{
	// Increase score
	size_t scoreOffset = lineCleared * (size_t)Settings::get().lineScore;

	//Multipliers
	if (lineCleared > 1) scoreOffset *= (lineCleared - 1) * (size_t)Settings::get().lineMultiplier;
	if (level > 1)       scoreOffset *= (level - 1)       * (size_t)Settings::get().levelMultipler;
	
	return scoreOffset;
}

void ScoreManager::reset_custom()
{
	score = 0;
	level = 0;
	levelProgress = 0;
}
