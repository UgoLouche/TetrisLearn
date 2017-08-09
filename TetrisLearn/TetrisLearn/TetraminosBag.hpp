#pragma once

#include <memory>
#include <random>

#include "const.hpp"

class Tetramino;
class IBoard;
class IBlocksPool;

class TetraminosBag
{
public:
	TetraminosBag(IBoard& board, IBlocksPool& pool);

	bool isEmpty();
	std::shared_ptr<Tetramino> getNext();

	//void init(Board* board);
	//bool isInit();

	//void clear();

private:
	// Static
	static std::mt19937 rnd_engine;

	void fillBag();
	void randomizeBag();
	void bagSwap(int a, int b);

	IBoard& board;
	IBlocksPool& pool;

	std::shared_ptr<Tetramino> bag[BAG_SIZE];
	size_t next;
};