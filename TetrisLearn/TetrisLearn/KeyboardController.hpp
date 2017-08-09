#pragma once

#include "SFML\Graphics.hpp"

#include "const.hpp"
#include "EnumeratedArray.hpp"

#include "Controller.hpp"
#include "TetrisTimer.hpp"
#include "KeyManager.hpp"

/*
* Controller based on keyboard inputs
*/
class KeyboardController : public Controller
{
public:
	KeyboardController(IControlledObject& controlled, TetrisGame& parent);
	virtual ~KeyboardController();

	void setKeysMapping(EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS> keysMapping);

private:
	EnumeratedArray<std::shared_ptr<KeyManager>, InputKeys, TOTAL_KEYS> keysManagers;

	void act_custom() override;

	uint32_t checkForKey(uint32_t input, InputKeys key);
};