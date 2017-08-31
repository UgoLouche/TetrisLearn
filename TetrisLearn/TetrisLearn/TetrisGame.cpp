
#include "ProtoEng\Layer.h"
#include "ProtoEng\gameView.h"
#include "ProtoEng\Box.h"

#include "const.hpp"

#include "TetrisGame.hpp"
#include "LayerType.hpp"
#include "Board.hpp"
#include "DummyController.hpp"
#include "LearningController.hpp"
#include "TetraThumbnail.hpp"
#include "SettingsWrapper.hpp"
#include "Settings.hpp"
#include "InputData.hpp"

TetrisGame::TetrisGame() :
	win(),
	world(),
	uiManager(),
	boards(),
	controllers(),
	blockPool()
{
}

TetrisGame::~TetrisGame()
{
	//Need to clear game layer first, or block manager (aka blockspool) won't exist when that happen
	world.getLayer(LayerType::GameEntity)->clearLayer();

	//Close InputData's file stream and prepare static fields for deletion
	dataFile.close();
}

void TetrisGame::Initialize()
{
	// Create the window.
	win.create(sf::VideoMode( Settings::get().winSize.x, Settings::get().winSize.y), Settings::get().winName);
	win.setActive();
	win.setGame(this);


	// Game Views.

	// Views are drawn in the same order as they are linked to win.
	//
	// Draw the UI FIRST. This is weird but UI Boxes are opaque so it would hide the whole game otherwise
	// This sucks, but ultimately it's a problem with ProtoEng, and I only use the dll here.
	ProtoEng::GameView* uiView = new ProtoEng::GameView();
	uiView->setSize(Settings::get().gridSize.x * PANEL_WIDTH, Settings::get().gridSize.y * PANEL_HEIGHT);
	uiView->setAnchor(sf::Vector2f(0.0f, 0.0f));
	uiView->setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
	uiView->assignLayer(world.getLayer(LayerType::GameUI));
	win.registerView(uiView);

	ProtoEng::GameView* gameView = new ProtoEng::GameView();
	gameView->setSize(Settings::get().gridSize.x * PANEL_BIG_BOX_WIDTH, Settings::get().gridSize.y * PANEL_HEIGHT);
	gameView->setAnchor(sf::Vector2f(0.0f, 0.0f));
	gameView->setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
	gameView->assignLayer(world.getLayer(LayerType::GameEntity));
	win.registerView(gameView);

	// Data collection
	dataFile.open(Settings::get().save_path, std::ofstream::out | std::ofstream::app);
	InputData::setFileStream(&dataFile);

	// Setup Boards.
	int index = 0;
	sf::Vector2f anchor = sf::Vector2f(0, 0);
	for (int i = 0; i < Settings::get().gridSize.x; ++i)
	{
		anchor.y = 0;
		for (int j = 0; j < Settings::get().gridSize.y; ++j)
		{
			panelSetup(anchor, index);
			anchor.y += PANEL_HEIGHT;
			++index;
		}
		anchor.x += PANEL_WIDTH;
	}
}

void TetrisGame::panelSetup(const sf::Vector2f anchor, size_t index)
{
	// Ui.
	ProtoEng::Box* bigBox = new ProtoEng::Box();
	ProtoEng::Box* smallBox = new ProtoEng::Box();

	bigBox->setPosition(anchor);
	bigBox->setDimensions(sf::Vector2f(PANEL_BIG_BOX_WIDTH, PANEL_BIG_BOX_HEIGHT));
	bigBox->setBorderProperties(PANEL_BOX_COLOR, PANEL_BOX_BORDER);
	world.registerElement(LayerType::GameUI, bigBox);

	smallBox->setPosition(anchor);
	smallBox->setDimensions(sf::Vector2f(PANEL_SMALL_BOX_WIDTH, PANEL_SMALL_BOX_HEIGHT));
	smallBox->setBorderProperties(PANEL_BOX_COLOR, PANEL_BOX_BORDER);
	world.registerElement(LayerType::GameUI, smallBox);


	// Texts.
	// Score.
	sf::Text* scoreText = new sf::Text();
	textSetup(
		scoreText, 
		LayerType::GameUI, 
		Settings::get().fontSize, 
		Settings::get().scoreText_string,
		anchor + sf::Vector2f(SCORE_TEXT_POS_X, SCORE_TEXT_POS_Y)
	);

	sf::Text* scoreCounter = new sf::Text();
	textSetup(
		scoreCounter,
		LayerType::GameUI,
		Settings::get().fontSize,
		Settings::get().scoreCounter_string,
		anchor + sf::Vector2f(SCORE_COUNTER_POS_X, SCORE_COUNTER_POS_Y)
	);

	//Levels.
	sf::Text* levelText = new sf::Text();
	textSetup(
		levelText,
		LayerType::GameUI,
		Settings::get().fontSize,
		Settings::get().levelText_string,
		anchor + sf::Vector2f(LEVEL_TEXT_POS_X, LEVEL_TEXT_POS_Y)
	);

	sf::Text* levelCounter = new sf::Text();
	textSetup(
		levelCounter, 
		LayerType::GameUI, 
		Settings::get().fontSize,
		Settings::get().levelCounter_string,
		anchor + sf::Vector2f(LEVEL_COUNTER_POS_X, LEVEL_COUNTER_POS_Y)
	);

	// GameOver display
	sf::Text* gameOverText = new sf::Text();
	textSetup(
		gameOverText, 
		LayerType::GameUI, 
		Settings::get().fontSize,
		"",
		anchor + sf::Vector2f(GAMEOVER_TEXT_POS_X, GAMEOVER_TEXT_POS_Y), 
		Settings::get().gameOver_color
	);


	// Thumbnails.
	// Hold thumbnail.
	sf::Text* holdText = new sf::Text();
	textSetup(
		holdText,
		LayerType::GameUI,
		Settings::get().thumbnails_fontSize,
		Settings::get().holdThumnail_string,
		anchor + sf::Vector2f(HOLD_ANCHOR_X, HOLD_ANCHOR_Y)
	);
	
	TetraThumbnail* hold = new TetraThumbnail(blockPool);
	hold->setPosition(anchor.x + HOLD_ANCHOR_X, anchor.y + HOLD_ANCHOR_Y + Settings::get().thumbnails_fontSize + 5);
	hold->changeColor(Settings::get().holdThumbnail_color);
	world.registerElement(LayerType::GameUI, hold);

	// Preview thumbnails
	sf::Text* previewText = new sf::Text();
	textSetup(
		previewText,
		LayerType::GameUI,
		Settings::get().thumbnails_fontSize,
		Settings::get().previewThumbnails_string,
		anchor + sf::Vector2f(PREVIEW_ANCHOR_X, PREVIEW_ANCHOR_Y)
	);

	TetraThumbnail* previews[THUMBNAIL_PREVIEW_NUMBER] = { new TetraThumbnail(blockPool), new TetraThumbnail(blockPool), new TetraThumbnail(blockPool) };
	for (int i = 0; i < THUMBNAIL_PREVIEW_NUMBER; ++i)
	{
		previews[i]->setPosition( anchor.x + PREVIEW_ANCHOR_X, anchor.y + PREVIEW_ANCHOR_Y + Settings::get().thumbnails_fontSize + 5 + i * PREVIEW_OFFSET);
		previews[i]->changeColor(Settings::get().previewsThumbnails_color);
		world.registerElement(LayerType::GameUI, previews[i]);
	}

	//Board Name
	sf::Text* nameText = new sf::Text();
	textSetup(
		nameText,
		LayerType::GameUI,
		Settings::get().boardName_fontSize,
		Settings::getBoard(index).name,
		anchor + sf::Vector2f(BOARDNAME_ANCHOR_X, BOARDNAME_ANCHOR_Y)
	);


	// Game
	// Board
	sf::Vector2f boardAnchor(anchor.x + PANEL_SMALL_BOX_WIDTH, anchor.y + PANEL_BOX_BORDER);
	boards.emplace_front(
		new Board(
			*scoreCounter,
			*levelCounter,
			*gameOverText,
			*hold,
			previews,
			blockPool,
			*this,
			boardAnchor,
			Settings::getBoard(index).recording,
			Settings::getBoard(index).isRecurring
		)
	);
	world.registerElement(LayerType::GameEntity, boards.front() );

	//Controls
	LearningController* controller = nullptr;
	switch (Settings::getBoard(index).control)
	{
	case InputMethod::Player:
	{
		KeyboardController* controller = nullptr;
		controller = new KeyboardController(*(boards.front()), *this);
		controller->setKeysMapping(Settings::getBoard(index).getUnion<const EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS>>());

		controllers.push_front(std::shared_ptr<Controller>(controller));
		break;
	}
	case InputMethod::Static_AI:
	case InputMethod::Learning:
		controller = new LearningController(*(boards.front()), *this, Settings::getBoard(index).getUnion<const std::string>());

		controllers.push_front(std::shared_ptr<Controller>(controller));
	default:
		break;
	}
}

void TetrisGame::gameplayUpdate_custom()
{
	for (auto it = boards.begin(); it != boards.end(); ++it)
		(*it)->Act();

	for (auto it = controllers.begin(); it != controllers.end(); ++it)
		(*it)->Act();
}

void TetrisGame::preUpdate_custom()
{
	for (auto it = boards.begin(); it != boards.end(); ++it)
		(*it)->notifyGraphicUpdate(deltaTime());
}

void TetrisGame::newEvent_custom(const sf::Event & ev)
{
	if (ev.type == sf::Event::Closed)
	{
		//Tag stuff for deletion
		for (auto it = controllers.begin(); it != controllers.end(); ++it)
			(*it)->deleteLater();

		//Tetramino::clearBags();
		win.close();
	}
}

void TetrisGame::textSetup(sf::Text * text, LayerType layer, unsigned int fontsize, sf::String str, sf::Vector2f pos, sf::Color color)
{
	text->setPosition(pos);
	text->setCharacterSize(fontsize);
	text->setString(str);
	text->setFont(uiManager.getFont());
	text->setColor(color);
	world.registerElement(layer, text);
}

void TetrisGame::launch()
{
	Initialize();

	win.launch();
}