#pragma once

#include "enum.hpp"

class IBoardUI
{
public:
	virtual ~IBoardUI();

	void changeScoreUI(size_t newScore);
	void changeLevelUI(size_t newLevel);
	
	void newHoldTetramino(TetraminoTypes type);;
	void setThumbnail(size_t index, TetraminoTypes type);

	void gameOverUI(bool isOver);

private:
	virtual void changeScoreUI_custom(size_t newScore) = 0;
	virtual void changeLevelUI_custom(size_t newLevel) = 0;
	
	virtual void newHoldTetramino_custom(TetraminoTypes type) = 0;
	virtual void setThumbnail_custom(size_t index, TetraminoTypes type) = 0;

	virtual void gameOverUI_custom(bool isOver) = 0;
};