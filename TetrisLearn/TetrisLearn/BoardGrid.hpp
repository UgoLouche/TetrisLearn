#pragma once

#include "const.hpp"

#include "IBoardContent.hpp"

class Block;
class IBoard;

class BoardGrid : public IBoardContent
{
public:
	BoardGrid(IBoard& parent);

private:
	Block* content[BOARD_WIDTH][BOARD_HEIGHT]; //Origin is bottom left corner.

	IBoard& parent;



	// Inherited via IBoardContent
	virtual bool postLockRowChecks_custom() override;
	virtual void removeRows_custom() override;

	virtual Block ** bracket_custom(int i) override;
	virtual Block * const * bracket_custom(int i) const override;

	virtual Block * getBlock_custom(sf::Vector2i pos) const override;
	virtual void setBlock_custom(Block * content, sf::Vector2i pos) override;
	virtual bool isFree_custom(sf::Vector2i pos) const override;

	// Ad-hoc
	bool checkRow(int index);
	void shiftRowDown(int index, int offset);

	bool validPos(sf::Vector2i pos) const;
};
