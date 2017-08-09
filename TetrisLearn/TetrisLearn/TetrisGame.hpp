#pragma once
#include <memory>
#include <list>
#include <fstream>

#include "SFML\Graphics.hpp"

#include "ProtoEng\gameplayManager.h"
#include "ProtoEng\GameWindow.h"
#include "ProtoEng\GameWorld.h"

#include "default.hpp"
#include "enum.hpp"

#include "UIManager.hpp"
#include "BlockPool.hpp"


class Board;
class Controller;

class TetrisGame : public ProtoEng::GameplayManager
{
public:
	TetrisGame();
	~TetrisGame();

	// Startup methods.
	void Initialize();
	void launch();

private:
	void panelSetup(const sf::Vector2f anchor, size_t index);

	ProtoEng::GameWindow win;
	ProtoEng::GameWorld world;
	UIManager uiManager;
	BlocksPool blockPool;


	std::list<Board*> boards; // Boards are destroyed through layers
	std::list<std::shared_ptr<Controller>> controllers;

	//Data collection
	std::ofstream dataFile = std::ofstream();

	// Overrides
	void gameplayUpdate_custom() override;
	void preUpdate_custom() override;
	void newEvent_custom(const sf::Event& ev);

	//Utility Functions.
	void textSetup(
		sf::Text* text, 
		LayerType layer, 
		unsigned int fontSize =     DEFAULT_TEXT_SIZE,
		sf::String str =            DEFAULT_TEXT_STRING, 
		sf::Vector2f pos =          DEFAULT_TEXT_POS, 
		sf::Color color =           DEFAULT_TEXT_COLOR
	);

};