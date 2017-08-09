#include "LearningController.hpp"

#include "Settings.hpp"
#include "enum.hpp"

#include "TetrisTimer.hpp"
#include "TetrisGame.hpp"
#include "IBoard.hpp"


LearningController::LearningController(IBoard& board, TetrisGame& game, std::string className) :
	Controller(board, game),
	board(board),
	pyWrapper(className)
{
	inputTimer = new TetrisTimer(Settings::get().DASTimer * 10); //Use DAS time (X2) for now, might be too fast
	game.registerTimedEvent(inputTimer);
	inputTimer->stop();
	inputTimer->reArm();
}

LearningController::~LearningController()
{
}

void LearningController::act_custom()
{
	//Dummy function for now, Simply make (fake) Python call and go left/right
	static bool goLeft = true;

	if (!pyWrapper.isLearning()) pyWrapper.startLearning();

	if (inputTimer->getFlag() == TimerFlag::Triggered)
	{
		//Call scoreMove and pick the best move
		InputKeys nextInput = selectBestMove();

		switch (nextInput)
		{
		case InputKeys::UP:
			controlled.inputDashDown();
			break;
		case InputKeys::LEFT:
			controlled.inputLeft();
			break;
		case InputKeys::RIGHT:
			controlled.inputRight();
			break;
		case InputKeys::DOWN:
			controlled.inputDown();
			break;
		case InputKeys::LFLIP:
			controlled.inputFlipL();
			break;
		case InputKeys::RFLIP:
			controlled.inputFlipR();
			break;
		case InputKeys::HOLD:
			controlled.inputHold();
			break;
		default:
			break;
		}

		inputTimer->stop();
		inputTimer->reArm();
	}
}

void LearningController::deleteLater_custom()
{
	pyWrapper.stopLearning();
}

InputKeys LearningController::selectBestMove()
{
	InputKeys it = InputKeys::UP;
	InputKeys ret = it;
	float best_score;
	float curr_score;

	do
	{
		curr_score = pyWrapper.scoreMove(board.getStrInputData(it));

		if (it == InputKeys::UP)
			best_score = curr_score;
		else if (curr_score > best_score)
		{
			best_score = curr_score;
			ret = it;
		}

		it++;
	} while (it == InputKeys::UP);

	return ret;
}

