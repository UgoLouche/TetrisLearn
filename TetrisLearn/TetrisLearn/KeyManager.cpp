#include "KeyManager.hpp"

#include "const.hpp"
#include "Settings.hpp"

KeyManager::KeyManager(TetrisGame& game, sf::Keyboard::Key mapped) :
	mapped(mapped),
	DASTimer(nullptr),
	DASCounter(0),
	forwardInput(false),
	timerRunning(false)
{
	DASTimer = new TetrisTimer(Settings::get().DASTimer);
	game.registerTimedEvent(DASTimer);

	DASTimer->stop();
}

bool KeyManager::getState()
{
	return forwardInput;
}

void KeyManager::act_custom()
{
	bool keyState = sf::Keyboard::isKeyPressed(mapped);

	if (keyState)
	{
		// Is timer running Running ?
		if (timerRunning)
		{
			// Is Timer trigerred ?
			if (DASTimer->getFlag() == TimerFlag::Triggered)
			{
				// Record trigger
				++DASCounter;

				// Is DAS enabled ?
				if (DASCounter >= Settings::get().stepToDAS) // -> Move and re-Arm
				{
					DASTimer->stop();
					DASTimer->reArm();
					forwardInput = true;
				}
				else // -> re-arm and do nothing
				{
					DASTimer->stop();
					DASTimer->reArm();
					forwardInput = false;
				}
			}
			else // -> Input on cooldown
			{
				forwardInput = false;
			}
		}
		else // -> Record keystroke, and start timer.
		{
			DASTimer->stop();
			DASTimer->reArm();
			timerRunning = true;
			forwardInput = true;
		}
	}
	else // Basically reset
	{
		forwardInput = false;
		DASTimer->stop();
		timerRunning = false;
		DASCounter = 0;
	}
}
