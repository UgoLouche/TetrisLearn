#include "IBlocksPool.hpp"

IBlocksPool::~IBlocksPool()
{
}

Block * IBlocksPool::getBlock()
{
	return getBlock_custom();
}

void IBlocksPool::storeBlock(Block * block)
{
	storeBlock_custom(block);
}
