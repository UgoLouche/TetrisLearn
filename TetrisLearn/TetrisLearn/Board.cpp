#include "Board.hpp"

#include <string>

#include "ITetraminoManager.hpp"
#include "TetraminoManager.hpp"
#include "BoardUIManager.hpp"
#include "Tetramino.hpp"
#include "BoardGrid.hpp"
#include "ScoreManager.hpp"
#include "TetrisGame.hpp"
#include "IBoardUI.hpp"
#include "Settings.hpp"

Board::Board(sf::Text& scoreCounter, sf::Text& levelCounter, sf::Text& gameOverText, TetraThumbnail& hold, TetraThumbnail* previews[THUMBNAIL_PREVIEW_NUMBER],
	IBlocksPool& blockPool, TetrisGame& parent, const sf::Vector2f anchor, bool isRecording, bool isRecurring) :
	parent(parent),
	state(BoardStates::Play), 
	tetraminos(*this, blockPool),
	lockTimer(new TetrisTimer(Settings::get().lockTime)),
	autoScrollTimer(new TetrisTimer(Settings::get().autoscollTime)),
	flashTimer(new TetrisTimer(Settings::get().flashTime)),
	uiManager(scoreCounter, levelCounter, gameOverText, hold, previews),
	blockPool(blockPool),
	content(*this),
	score(*this),
	isRecording(isRecording),
	isRecurring(isRecurring)
{
	// Register timers
	parent.registerTimedEvent(lockTimer);
	parent.registerTimedEvent(autoScrollTimer);
	parent.registerTimedEvent(flashTimer);

	// Initialize score display
	changeScoreUI(0);
	changeLevelUI(1);

	//Set board position in the world (ie the location of the bottom-left block)
	setPosition(anchor + sf::Vector2f(0, (BOARD_VISIBLE_HEIGHT - 1) * BLOCK_SIZE));
}

void Board::act_custom()
{
	switch (state)
	{
	case BoardStates::Play:

		if (tetraminos.current() == nullptr)
			tetraminos.activate_next();

		if (autoScrollTimer->getFlag() == TimerFlag::Triggered)
		{
			if (tetraminos.current()->canStepDown())
			{
				registerNewInput(Inputs::Down);
				tetraminos.current()->stepDown();
				recordNewPosition();

				autoScrollTimer->stop(); // Reset Flag.
				autoScrollTimer->reArm();
			}
			else
			{
				lockTimer->stop();
				lockTimer->reArm();

				changeState(BoardStates::WaitForLock);
			}
		}

		break;

	case BoardStates::WaitForLock:

		if (lockTimer->getFlag() == TimerFlag::Triggered)
		{
			tetraminos.current()->release();
			tetraminos.activate_next();

			if (content.postLockRowChecks())
			{
				flashTimer->stop();
				flashTimer->reArm();

				changeState(BoardStates::Flash);
			}
			else
			{
				autoScrollTimer->stop();
				autoScrollTimer->reArm();

				changeState(BoardStates::Play);
			}
		}

		break;

	case BoardStates::Flash:

		if (flashTimer->getFlag() == TimerFlag::Triggered)
		{
			content.removeRows();

			autoScrollTimer->stop();
			autoScrollTimer->reArm();

			changeState(BoardStates::Play);
		}

		break;

	case BoardStates::Over:
	default:
		break;
	}
}

void Board::changeScoreUI_custom(size_t newScore)
{
	uiManager.changeScoreUI(newScore);
}

void Board::changeLevelUI_custom(size_t newLevel)
{
	uiManager.changeLevelUI(newLevel);
}

void Board::newHoldTetramino_custom(TetraminoTypes type)
{
	uiManager.newHoldTetramino(type);
}

void Board::setThumbnail_custom(size_t index, TetraminoTypes type)
{
	uiManager.setThumbnail(index, type);
}

void Board::gameOverUI_custom(bool isOver)
{
	uiManager.gameOverUI(isOver);
}

bool Board::postLockRowChecks_custom()
{
	return content.postLockRowChecks();
}

void Board::removeRows_custom()
{
	content.removeRows();
}

Block ** Board::bracket_custom(int i)
{
	return content[i];
}

Block * const * Board::bracket_custom(int i) const
{
	return content[i];
}

void Board::addScore_custom(size_t lineCleared)
{
	score.addScore(lineCleared);

	recordNewScore(lineCleared);
}

size_t Board::getScore_custom() const
{
	return score.getScore();
}

size_t Board::getLevel_custom() const
{
	return score.getLevel();
}

Block * Board::getBlock_custom()
{
	return blockPool.getBlock();
}

void Board::storeBlock_custom(Block * block)
{
	blockPool.storeBlock(block);
}

void Board::gameOver_custom()
{
	if (!isRecurring)
	{
		changeState(BoardStates::Over);
		uiManager.gameOverUI(true);
	}
	else //reboot
		reset_custom();
}

std::string Board::getStrInputData_custom(InputKeys overrideInput)
{
	if (current_move == nullptr) return "";

	return current_move->toString(true, overrideInput);
}

void Board::reset_custom()
{
	changeScoreUI(0);
	changeLevelUI(1);
	score.reset();
	content.reset();
}

void Board::registerNewInput(Inputs input)
{
	size_t prevID = current_move == nullptr ? 0 : current_move->getID();

	current_move = std::make_shared<InputData>(tetraminos, content, input, prevID, isRecording);
}

void Board::recordNewPosition()
{
	if (isRecording && current_move != nullptr)
	{
		current_move->setNewPosition(*tetraminos.current());
	}
}

void Board::recordNewScore(size_t linesCleared)
{
	if (isRecording && current_move != nullptr)
	{
		current_move->setScore(linesCleared, simulateScore(linesCleared, 1));
	}
}

size_t Board::simulateScore_custom(size_t lineCleared, size_t level) const
{
	return score.simulateScore(lineCleared, level);
}

bool Board::isFree_custom(sf::Vector2i pos) const
{
	return content.isFree(pos);
}

Block * Board::getBlock_custom(sf::Vector2i pos) const
{
	return content.getBlock(pos);
}

void Board::setBlock_custom(Block * content, sf::Vector2i pos)
{
	this->content.setBlock(content, pos);
}

void Board::notifyGraphicUpdate_custom(sf::Time deltaTime)
{
	if (tetraminos.current() != nullptr) tetraminos.current()->notifyGraphicUpdate(deltaTime);

	for (int i = 0; i < BOARD_VISIBLE_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			if (content[j][i] != nullptr) content[j][i]->notifyGraphicUpdate(deltaTime);
		}
	}
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Transform combinedTransform = states.transform * getTransform();

	if (tetraminos.current() != nullptr) target.draw(*(tetraminos.current()), combinedTransform);

	for (int i = 0; i < BOARD_VISIBLE_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			if (content[j][i] != nullptr) target.draw(*(content[j][i]), combinedTransform);
		}
	}
}

// TODO Change Inputs by InputsKey the two are redundants ...
void Board::input_custom(uint32_t inputCode)
{
	// Cases where input is ignored
	if (tetraminos.current() == nullptr) return;

	if (state == BoardStates::Flash || state == BoardStates::Over) return;

	// Process all inputs
	// In case of composite inputs: create a new input data for each (thus immediately delete and write down the last one)
	if (inputCode & Inputs::DashDown)
	{
		registerNewInput(Inputs::DashDown);
		while (tetraminos.current()->canStepDown()) tetraminos.current()->stepDown();
		recordNewPosition();

		// Insta-lock
		// Go to lock state, and insta trigger the timer.
		changeState(BoardStates::WaitForLock);
		lockTimer->stop();
		lockTimer->reArm();
		lockTimer->check(lockTimer->getBaseTime());
	}

	if (inputCode & Inputs::DashLeft)
	{
	}

	if (inputCode & Inputs::DashRight)
	{
	}

	if (inputCode & Inputs::Down)
	{
		if (tetraminos.current()->canStepDown())
		{
			registerNewInput(Inputs::Down);
			tetraminos.current()->stepDown();
			recordNewPosition();
		}
	}

	if (inputCode & Inputs::FlipR)
	{
		if (tetraminos.current()->canFlipR())
		{
			registerNewInput(Inputs::FlipR);
			tetraminos.current()->flipR();
			recordNewPosition();
		}
	}

	if (inputCode & Inputs::FlipL)
	{
		if (tetraminos.current()->canFlipL())
		{
			registerNewInput(Inputs::FlipL);
			tetraminos.current()->flipL();
			recordNewPosition();
		}
	}

	if (inputCode & Inputs::Hold)
	{
		registerNewInput(Inputs::Hold);
		tetraminos.holdTetramino();
		recordNewPosition();
	}

	if (inputCode & Inputs::Left)
	{
		if (tetraminos.current()->canStepLeft())
		{
			registerNewInput(Inputs::Left);
			tetraminos.current()->stepLeft();
			recordNewPosition();
		}
	}

	if (inputCode & Inputs::Right)
	{
		if (tetraminos.current()->canStepRight())
		{
			registerNewInput(Inputs::Right);
			tetraminos.current()->stepRight();
			recordNewPosition();
		}
	}

	// Check if we got out of lock mode
	if (state == BoardStates::WaitForLock && tetraminos.current()->canStepDown())
	{
		autoScrollTimer->stop();
		autoScrollTimer->reArm();

		changeState(BoardStates::Play);
	}
}

void Board::changeState(BoardStates newState, bool force)
{
	if (state == BoardStates::Over && !force) return;

	state = newState;
}

/*void Board::setBlock(Block* block, sf::Vector2i pos)
{
	content[pos.x][pos.y] = block;
}

bool Board::isFree(sf::Vector2i pos)
{
	if (pos.x < 0 || pos.x >= BOARD_WIDTH) return false;
	if (pos.y < 0 || pos.y >= BOARD_HEIGHT) return false;
	if (content[pos.x][pos.y] != nullptr) return false;
	else return true;
}*/

/*
size_t Board::getID()
{
	return boardID;
}
*/

/*
BlocksPool& Board::getBlockPool()
{
	return blockPool;
}
*/

/*
void Board::clear()
{
	if (current_tetramino != nullptr) current_tetramino->clear();

	nexts.pop()->clear();
	nexts.pop()->clear();
	nexts.pop()->clear();

	if (hold_tetramino != nullptr) hold_tetramino->clear();

	for (int i = 0; i < BOARD_WIDTH; ++i)
	{
		for (int j = 0; j < BOARD_HEIGHT; ++j)
		{
			if (content[i][j] != nullptr)
			{
				blockPool.storeBlock(content[i][j]);
				content[i][j] = nullptr;
			}
		}
	}
}
*/
