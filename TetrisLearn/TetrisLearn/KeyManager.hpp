#pragma once

#include "SFML\Graphics.hpp"

#include "IActingObject.hpp"
#include "TetrisGame.hpp"
#include "TetrisTimer.hpp"

// Kepp track of a single Key, and its DAS timer and so on... 
class KeyManager : public IActingObject
{
public:
	KeyManager(TetrisGame& game, sf::Keyboard::Key mapped);

	bool getState();

private:
	TetrisTimer* DASTimer;
	int DASCounter;
	bool timerRunning;

	bool forwardInput;

	sf::Keyboard::Key mapped;

	//Override
	void act_custom() override;

};