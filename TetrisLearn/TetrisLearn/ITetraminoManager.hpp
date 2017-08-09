#pragma once

#include "enum.hpp"

class Tetramino;

class ITetraminoManager
{
public:
	virtual ~ITetraminoManager();

	Tetramino* current() const;
	void activate_next();
	void holdTetramino();

	//Some Getter
	TetraminoTypes getNextType(size_t index) const;
	TetraminoTypes getHoldType() const;
	bool isHolding() const;

private:
	virtual Tetramino* current_custom() const = 0;
	virtual void activate_next_custom() = 0;
	virtual void holdTetramino_custom() = 0;

	virtual TetraminoTypes getNextType_custom(size_t index) const = 0;
	virtual TetraminoTypes getHoldType_custom() const = 0;
	virtual bool isHolding_custom() const = 0;
};