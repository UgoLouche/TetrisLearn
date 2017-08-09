#include "IBoardUI.hpp"

IBoardUI::~IBoardUI()
{
}

void IBoardUI::changeScoreUI(size_t newScore)
{
	changeScoreUI_custom(newScore);
}

void IBoardUI::changeLevelUI(size_t newLevel)
{
	changeLevelUI_custom(newLevel);
}

void IBoardUI::newHoldTetramino(TetraminoTypes type)
{
	newHoldTetramino_custom(type);
}

void IBoardUI::setThumbnail(size_t index, TetraminoTypes type)
{
	setThumbnail_custom(index, type);
}

void IBoardUI::gameOverUI(bool isOver)
{
	gameOverUI_custom(isOver);
}
