#include "TetraminosBag.hpp"

#include <chrono>

#include "enum.hpp"

#include "Tetramino.hpp"

std::mt19937 TetraminosBag::rnd_engine = std::mt19937(
	(unsigned int) ( std::chrono::system_clock::now().time_since_epoch().count() )
);

TetraminosBag::TetraminosBag(IBoard& board, IBlocksPool& pool) :
	board(board), pool(pool)
{
	fillBag();
}

bool TetraminosBag::isEmpty()
{
	return (next == BAG_SIZE);
}

std::shared_ptr<Tetramino> TetraminosBag::getNext()
{
	if (isEmpty()) fillBag();

	return bag[next++];
}

void TetraminosBag::fillBag()
{
	next = 0;

	TetraminoTypes baseType = TetraminoTypes::I;
	for (int i = 0; i < BAG_SIZE; ++i)
		bag[i].reset(new Tetramino(board, pool, false, baseType++));

	randomizeBag();
}

void TetraminosBag::randomizeBag()
{
	std::uniform_int_distribution<int> distri;
	for (int i = 0; i < BAG_SIZE - 1; ++i)
	{
		distri = std::uniform_int_distribution<int>(i, BAG_SIZE - 1);
		bagSwap(i, distri(rnd_engine));
	}
}

void TetraminosBag::bagSwap(int a, int b)
{
	auto tmp = bag[a];
	bag[a] = bag[b];
	bag[b] = tmp;
}
