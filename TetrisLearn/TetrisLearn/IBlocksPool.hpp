#pragma once

#include "Block.hpp"

class IBlocksPool
{
public:
	virtual ~IBlocksPool();

	Block* getBlock();
	void storeBlock(Block* block);

private:
	virtual Block* getBlock_custom() = 0;
	virtual void storeBlock_custom(Block* block) = 0;
};