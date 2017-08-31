#pragma once

#include "SFML\Graphics.hpp"

class Block;

class IBoardContent
{
public:
	virtual ~IBoardContent();

	bool postLockRowChecks(); // Return true if a row is completed
	void removeRows();
	void reset();

	//Operator
	Block** operator[](int i);
	Block* const * operator[](int i) const;

	// Those checks for boundaries.
	bool isFree(sf::Vector2i pos) const;
	Block* getBlock(sf::Vector2i pos) const;
	void setBlock(Block* content, sf::Vector2i pos);


private:
	virtual bool postLockRowChecks_custom() = 0;
	virtual void removeRows_custom() = 0;

	//Operator
	virtual Block** bracket_custom(int i) = 0;
	virtual Block* const * bracket_custom(int i) const = 0;

	//Check index correctness
	virtual bool isFree_custom(sf::Vector2i pos) const = 0;
	virtual Block* getBlock_custom(sf::Vector2i pos) const = 0;
	virtual void setBlock_custom(Block* content, sf::Vector2i pos) = 0;

	//Reset
	virtual void reset_custom() = 0;
};
