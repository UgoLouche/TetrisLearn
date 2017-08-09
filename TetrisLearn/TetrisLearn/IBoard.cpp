#include "IBoard.hpp"

IBoard::~IBoard()
{
}

void IBoard::gameOver()
{
	gameOver_custom();
}

std::string IBoard::getStrInputData(InputKeys overrideInput)
{
	return getStrInputData_custom(overrideInput);
}
