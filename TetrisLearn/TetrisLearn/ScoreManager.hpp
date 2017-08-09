#pragma once

#include "IBoardScore.hpp"

class IBoard;

class ScoreManager : public IBoardScore
{
public:
	ScoreManager(IBoard& parent);

private:
	size_t score = 0;
	size_t level = 0;
	size_t levelProgress = 0;

	IBoard& parent;

	// Inherited via IBoardScore
	virtual void addScore_custom(size_t lineCleared) override;

	virtual size_t getScore_custom() const override;
	virtual size_t getLevel_custom() const override;


	// Ad-hoc
	void setScore(size_t newScore);
	void setLevel(size_t newLevel);


	// Inherited via IBoardScore
	virtual size_t simulateScore_custom(size_t lineCleared, size_t level) const override;

};