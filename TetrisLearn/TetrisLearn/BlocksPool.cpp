#include "BlockPool.hpp"

#include "const.hpp"

#include "Settings.hpp"

BlocksPool::BlocksPool() :
	blockList(),
	availableBlocks(),
	blockTexture()
{
	blockTexture.loadFromFile(Settings::get().blockTexturePath);
	increaseCapacity();
}

void BlocksPool::increaseCapacity()
{
	size_t addedBlocks = 0;

	if (availableBlocks.size() == 0) addedBlocks = Settings::get().blockPoolSize;
	else addedBlocks = availableBlocks.size();

	for (int i = 0; i < addedBlocks; ++i)
		newBlock();
}

void BlocksPool::newBlock()
{
	blockList.emplace_front();
	blockList.front().setSprite(sf::Sprite(blockTexture));
	availableBlocks.push( &( blockList.front() ) );
}

Block * BlocksPool::getBlock_custom()
{
	if (availableBlocks.empty()) increaseCapacity();

	Block* ret = availableBlocks.top();
	availableBlocks.pop();

	return ret;
}

void BlocksPool::storeBlock_custom(Block * block)
{
	availableBlocks.push(block);
}


