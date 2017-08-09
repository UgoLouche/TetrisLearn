#include "KeyboardController.hpp"

KeyboardController::KeyboardController(IControlledObject & controlled, TetrisGame& parent) :
	Controller(controlled, parent)
{
}

KeyboardController::~KeyboardController()
{
}

void KeyboardController::setKeysMapping(EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS> keysMapping)
{
	keysManagers[InputKeys::UP]    .reset(new KeyManager(parent, keysMapping[InputKeys::UP]));
	keysManagers[InputKeys::LEFT]  .reset(new KeyManager(parent, keysMapping[InputKeys::LEFT]));
	keysManagers[InputKeys::RIGHT] .reset(new KeyManager(parent, keysMapping[InputKeys::RIGHT]));
	keysManagers[InputKeys::DOWN]  .reset(new KeyManager(parent, keysMapping[InputKeys::DOWN]));
	keysManagers[InputKeys::LFLIP] .reset(new KeyManager(parent, keysMapping[InputKeys::LFLIP]));
	keysManagers[InputKeys::RFLIP] .reset(new KeyManager(parent, keysMapping[InputKeys::RFLIP]));
	keysManagers[InputKeys::HOLD]  .reset(new KeyManager(parent, keysMapping[InputKeys::HOLD]));
}

void KeyboardController::act_custom()
{
	uint32_t input = 0;

	input = checkForKey(input, InputKeys::UP);
	input = checkForKey(input, InputKeys::LEFT);
	input = checkForKey(input, InputKeys::RIGHT);
	input = checkForKey(input, InputKeys::DOWN);
	input = checkForKey(input, InputKeys::LFLIP);
	input = checkForKey(input, InputKeys::RFLIP);
	input = checkForKey(input, InputKeys::HOLD);

	controlled.input(input);
}

uint32_t KeyboardController::checkForKey(uint32_t input, InputKeys key)
{
	keysManagers[key]->Act();
	if (keysManagers[key]->getState())
	{
		switch (key)
		{
		case InputKeys::UP:
			input = input | Inputs::DashDown;
			break;
		case InputKeys::LEFT:
			input = input | Inputs::Left;
			break;
		case InputKeys::RIGHT:
			input = input | Inputs::Right;
			break;
		case InputKeys::DOWN:
			input = input | Inputs::Down;
			break;
		case InputKeys::LFLIP:
			input = input | Inputs::FlipL;
			break;
		case InputKeys::RFLIP:
			input = input | Inputs::FlipR;
			break;
		case InputKeys::HOLD:
			input = input | Inputs::Hold;
			break;
		default:
			break;
		}
	}

	return input;
}


