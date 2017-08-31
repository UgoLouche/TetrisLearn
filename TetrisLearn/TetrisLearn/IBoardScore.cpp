#include "IBoardScore.hpp"

IBoardScore::~IBoardScore()
{
}

void IBoardScore::addScore(size_t lineCleared)
{
	addScore_custom(lineCleared);
}

size_t IBoardScore::simulateScore(size_t lineCleared, size_t level) const
{
	return simulateScore_custom(lineCleared, level);
}

size_t IBoardScore::getScore() const
{
	return getScore_custom();
}

size_t IBoardScore::getLevel() const
{
	return getLevel_custom();
}

void IBoardScore::reset()
{
	reset_custom();
}
