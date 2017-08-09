#pragma once

#include <list>
#include <stack>

#include "SFML\Graphics.hpp"

#include "Block.hpp"
#include "IBlocksPool.hpp"

class ISettingsWrapper;
class BlocksPool : public IBlocksPool
{
public:
	BlocksPool();

private:
	std::list<Block> blockList; //List of all created block. Easier ownership management.
	std::stack<Block*> availableBlocks; 
	sf::Texture blockTexture;

	void increaseCapacity();
	void newBlock();


	// Inherited via IBlocksPool
	virtual Block * getBlock_custom() override;
	virtual void storeBlock_custom(Block * block) override;

};
