#include "DummyController.hpp"

DummyController::DummyController(IControlledObject& controlled, TetrisGame& parent) :
	KeyboardController(controlled, parent)
{
	setKeysMapping(
		EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS>(
		{
			DEFAULT_KEY_UP,
			DEFAULT_KEY_LEFT,
			DEFAULT_KEY_RIGHT,
			DEFAULT_KEY_DOWN,
			DEFAULT_KEY_LFLIP,
			DEFAULT_KEY_RFLIP,
			DEFAULT_KEY_HOLD
		})
	);
}