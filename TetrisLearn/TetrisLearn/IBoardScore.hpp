#pragma once

class IBoardScore
{
public:
	virtual ~IBoardScore();

	void addScore(size_t lineCleared);

	size_t simulateScore(size_t lineCleared, size_t level) const;
	size_t getScore() const;
	size_t getLevel() const;

private:
	virtual void addScore_custom(size_t lineCleared) = 0;

	virtual size_t simulateScore_custom(size_t lineCleared, size_t level) const = 0;
	virtual size_t getScore_custom() const = 0;
	virtual size_t getLevel_custom() const = 0;
};