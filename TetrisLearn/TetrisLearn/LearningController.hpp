#pragma once

#include "enum.hpp"

#include "Controller.hpp"
#include "PythonWrapper.hpp"

class IBoard;
class TetrisGame;
class TetrisTimer;

class LearningController : public Controller
{
public:
	LearningController(IBoard& board, TetrisGame& game, std::string className);
	virtual ~LearningController();

private:
	IBoard& board;
	
	TetrisTimer* inputTimer;

	//Python Calls
	PythonWrapper pyWrapper;

	// Inherited via Controller
	virtual void act_custom() override;

	// Inherited via IActingObject
	virtual void deleteLater_custom() override;

	//Ad-hoc
	InputKeys selectBestMove();
};