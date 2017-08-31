#include "SettingsWrapper.hpp"

#include "pugiXML\pugixml.hpp"

#include "default.hpp"
#include "markup.hpp"
#include "enum.hpp"

SettingsWrapper::SettingsWrapper()
{
	//Just to be sure
	settings.boardCount = 0;
	settings.boardsArray = nullptr;

	loadDefault();
}

SettingsWrapper::~SettingsWrapper()
{
	clearBoards();
}

bool SettingsWrapper::loadSettings(std::string path)
{
	pugi::xml_document doc;
	if (!doc.load_file(path.c_str())) return false;

	for (auto it = doc.begin(); it != doc.end(); ++it)
	{
		//Ignore everything except the FIRST <game> markup
		if (strcmp(it->name(), MARKUP_GAME) == 0)
		{
			return parseGame(*it);
		}
	}

	loadDefault();
	return false;
}

bool SettingsWrapper::loadDefault()
{
	settings.winSize = sf::Vector2i(DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT);
	settings.winName = DEFAULT_WIN_NAME;

	settings.gridSize = sf::Vector2i(DEFAULT_NB_BOARDS_HOR, DEFAULT_NB_BOARD_VER);

	settings.textColor = DEFAULT_TEXT_COLOR;
	settings.textPosition = DEFAULT_TEXT_POS;
	settings.textString = DEFAULT_TEXT_STRING;
	settings.textSize = DEFAULT_TEXT_SIZE;

	settings.scoreText_string = DEFAULT_SCORE_TEXT;
	settings.scoreCounter_string = DEFAULT_SCORE_COUNTER;
	settings.levelText_string = DEFAULT_LEVEL_TEXT;
	settings.levelCounter_string = DEFAULT_LEVEL_COUNTER;

	settings.gameOver_string = DEFAULT_GAMEOVER_TEXT;
	settings.gameOver_color = DEFAULT_GAMEOVER_COLOR;

	settings.holdThumbnail_color = DEFAULT_THUMBNAIL_COLOR_HOLD;
	settings.previewsThumbnails_color = DEFAULT_THUMBNAIL_COLOR_PREVIEW;

	settings.holdThumnail_string = DEFAULT_HOLD_TEXT;
	settings.previewThumbnails_string = DEFAULT_PREVIEW_TEXT;

	settings.thumbnails_fontSize = DEFAULT_THUMBNAIL_FONT_SIZE;

	settings.boardName_fontSize = DEFAULT_BOARDNAME_FONTSIZE;

	settings.fontPath = DEFAULT_FONT_FILE;
	settings.fontSize = DEFAULT_FONT_SIZE;

	settings.lockTime = DEFAULT_LOCK_TIME_BASE;
	settings.autoscollTime = DEFAULT_AUTOSCROLL_TIME_BASE;
	settings.flashTime = DEFAULT_FLASH_TIME;
	settings.DASTimer = DEFAULT_DAS_TIMER;
	settings.stepToDAS = DEFAULT_STEP_TO_ENABLE_DAS;

	settings.flashAnimDelay = DEFAULT_FLASH_ANIM_DELAY;
	settings.activeSprite_color = DEFAULT_SPRITE_COLOR_ACTIVE;
	settings.lockedSprite_color = DEFAULT_SPRITE_COLOR_LOCKED;
	settings.flashingSprite_color = DEFAULT_SPRITE_COLOR_FLASHING_1;
	settings.flashingSprite_altColor = DEFAULT_SPRITE_COLOR_FLASHING_2;
	settings.ghostSprite_color = DEFAULT_SPRITE_COLOR_GHOST;

	settings.blockPoolSize = DEFAULT_BLOCKPOOL_SIZE_BASE;
	settings.blockTexturePath = DEFAULT_BLOCK_TEXTURE;

	settings.lineScore = DEFAULT_LINE_SCORE;
	settings.lineMultiplier = DEFAULT_LINE_MULTIPLIER;
	settings.levelMultipler = DEFAULT_LEVEL_SCORE_MULTIPLIER;
	settings.levelSpeed = DEFAULT_LEVEL_SPEED_MULTIPLIER;
	settings.linesToLevelup = DEFAULT_LINES_TO_LEVELUP;

	settings.value_separator = DEFAULT_VAL_SEPARATOR;
	settings.save_path = DEFAULT_SAVE_PATH;

	//Boards
	resetBoard(settings.defaultBoard);

	allocateBoards();

	return true;
}

void SettingsWrapper::allocateBoards()
{
	// Cleanup first.
	clearBoards();

	settings.boardCount = settings.gridSize.x * settings.gridSize.y;
	settings.boardsArray = new BoardSettings*[settings.boardCount];
	for (int i = 0; i < settings.boardCount; ++i)
	{
		settings.boardsArray[i] = nullptr;
	}
}

void SettingsWrapper::clearBoards()
{
	if (settings.boardCount != 0)
	{
		for (int i = 0; i < settings.boardCount; ++i)
		{
			if (settings.boardsArray[i] != nullptr)  delete settings.boardsArray[i];
		}

		delete[] settings.boardsArray;
	}
}

bool SettingsWrapper::parseGame(const pugi::xml_node & gameNode)
{
	bool ret = true;

	for (auto it = gameNode.begin(); it != gameNode.end(); ++it)
	{
		// Easy ones are processed directly here
		// More complicated ones have their own method for it

		//<window>
		if (strcmp(it->name(), MARKUP_WINDOW) == 0)
		{
			for (auto inner = it->begin(); inner != it->end(); ++inner)
			{
				if (strcmp(inner->name(), MARKUP_NAME) == 0)
				{
					settings.winName = parseName(*inner, settings.winName);
				}
				else if (strcmp(inner->name(), MARKUP_SIZE) == 0)
				{
					settings.winSize = parseSize(*inner, settings.winSize);
				}
			}
		}

		//<grid>
		else if (strcmp(it->name(), MARKUP_GRID) == 0)
		{
			for (auto inner = it->begin(); inner != it->end(); ++inner)
			{
				if (strcmp(inner->name(), MARKUP_SIZE) == 0)
				{
					settings.gridSize = parseSize(*inner, settings.gridSize);
				}
			}

			allocateBoards();
		}

		//<text>
		else if (strcmp(it->name(), MARKUP_TEXT) == 0)
		{
			for (auto inner = it->begin(); inner != it->end(); ++inner)
			{
				if (strcmp(inner->name(), MARKUP_COLOR) == 0)
				{
					settings.textColor = parseColor(*inner, settings.textColor);
				}
				else if (strcmp(inner->name(), MARKUP_POSITION) == 0)
				{
					settings.textPosition = parsePosition(*inner, settings.textPosition);
				}
				else if (strcmp(inner->name(), MARKUP_STRING) == 0)
				{
					settings.textString = parseString(*inner, settings.textString);
				}
				else if (strcmp(inner->name(), MARKUP_FONTSIZE) == 0)
				{
					settings.textSize = parseSize(*inner, settings.textSize);
				}
			}
		}

		//<UI>
		else if (strcmp(it->name(), MARKUP_UI) == 0)
		{
			ret = ret && parseUI(*it);
		}

		//<font>
		else if (strcmp(it->name(), MARKUP_FONT) == 0)
		{
			for (auto inner = it->begin(); inner != it->end(); ++inner)
			{
				if (strcmp(inner->name(), MARKUP_FONTSIZE) == 0)
				{
					settings.fontSize = parseSize(*inner, settings.fontSize);
				}
				else if (strcmp(inner->name(), MARKUP_FILE) == 0)
				{
					settings.fontPath = parsePath(*inner, settings.fontPath);
				}
			}
		}

		//<rules>
		else if (strcmp(it->name(), MARKUP_RULES) == 0)
		{
			ret = ret && parseRules(*it);
		}

		//<visual>
		else if (strcmp(it->name(), MARKUP_VISUAL) == 0)
		{
			ret - ret && parseVisual(*it);
		}

		//<block>
		else if (strcmp(it->name(), MARKUP_BLOCK) == 0)
		{
			for (auto inner = it->begin(); inner != it->end(); ++inner)
			{
				if (strcmp(inner->name(), MARKUP_POOLSIZE) == 0)
				{
					settings.blockPoolSize = parsePoolsize(*inner, settings.blockPoolSize);
				}
				else if (strcmp(inner->name(), MARKUP_TEXTURE) == 0)
				{
					settings.blockTexturePath = parsePath(*inner, settings.blockTexturePath);
				}
			}
		}

		//<score>
		else if (strcmp(it->name(), MARKUP_SCORE) == 0)
		{
			ret = ret && parseScore(*it);
		}

		//<recording>
		else if (strcmp(it->name(), MARKUP_RECORDING) == 0)
		{
			for (auto inner = it->begin(); inner != it->end(); ++inner)
			{
				if (strcmp(inner->name(), MARKUP_PATH) == 0)
				{
					settings.save_path = parsePath(*inner, settings.save_path);
				}
				else if (strcmp(inner->name(), MARKUP_SEPARATOR) == 0)
				{
					settings.value_separator = parseChar(*inner, settings.value_separator);
				}
			}
		}

		//<boards>
		else if (strcmp(it->name(), MARKUP_BOARDS) == 0)
		{
			ret = ret && parseBoards(*it);
		}
	}

	return ret;
}

bool SettingsWrapper::parseUI(const pugi::xml_node & UINode)
{
	for (auto it = UINode.begin(); it != UINode.end(); ++it)
	{
		if (strcmp(it->name(), MARKUP_SCORE) == 0)
		{
			for (auto inner = it->begin(); inner != it->end(); ++inner)
			{
				if (strcmp(it->name(), MARKUP_STRING) == 0)
					settings.scoreText_string = parseString(*inner, settings.scoreText_string);
				else if (strcmp(it->name(), MARKUP_COUNTER) == 0)
					settings.scoreCounter_string = parseCounter(*inner, settings.scoreCounter_string);
			}
		}

		else if (strcmp(it->name(), MARKUP_LEVEL) == 0)
		{
			for (auto inner = it->begin(); inner != it->end(); ++inner)
			{
				if (strcmp(it->name(), MARKUP_STRING) == 0)
					settings.levelText_string = parseString(*inner, settings.levelText_string);
				else if (strcmp(it->name(), MARKUP_COUNTER) == 0)
					settings.levelCounter_string = parseString(*inner, settings.levelCounter_string);
			}
		}

		else if (strcmp(it->name(), MARKUP_THUMBNAILS) == 0)
		{
			for (auto inner = it->begin(); inner != it->end(); ++inner)
			{
				if (strcmp(it->name(), MARKUP_HOLD) == 0)
				{
					for (auto innerInner = inner->begin(); innerInner != inner->end(); ++innerInner)
					{
						if (strcmp(it->name(), MARKUP_STRING) == 0)
							settings.holdThumnail_string = parseString(*innerInner, settings.holdThumnail_string);
						else if (strcmp(it->name(), MARKUP_COLOR) == 0)
							settings.holdThumbnail_color = parseColor(*innerInner, settings.holdThumbnail_color);
					}
				}

				else if (strcmp(it->name(), MARKUP_SCORE) == 0)
				{
					for (auto innerInner = inner->begin(); innerInner != inner->end(); ++innerInner)
					{
						if (strcmp(it->name(), MARKUP_STRING) == 0)
							settings.previewThumbnails_string = parseString(*innerInner, settings.previewThumbnails_string);
						else if (strcmp(it->name(), MARKUP_COLOR) == 0)
							settings.previewsThumbnails_color = parseColor(*innerInner, settings.previewsThumbnails_color);
					}
				}

				else if (strcmp(it->name(), MARKUP_FONTSIZE) == 0)
					settings.thumbnails_fontSize = parseSize(*inner, settings.thumbnails_fontSize);
			}
		}
	}

	return true;
}

bool SettingsWrapper::parseRules(const pugi::xml_node & rulesNode)
{
	for (auto it = rulesNode.begin(); it != rulesNode.end(); ++it)
	{
		if (strcmp(it->name(), MARKUP_DELAYS) == 0)
		{
			for (auto inner = it->begin(); inner != it->end(); ++inner)
			{
				if (strcmp(it->name(), MARKUP_LOCK) == 0)
					settings.lockTime = parseLock(*inner, settings.lockTime);
				else if (strcmp(it->name(), MARKUP_AUTOSCROLL) == 0)
					settings.autoscollTime = parseAutoScroll(*inner, settings.autoscollTime);
				else if (strcmp(it->name(), MARKUP_FLASH) == 0)
					settings.flashTime = parseFlash(*inner, settings.flashTime);
			}
		}

		else if (strcmp(it->name(), MARKUP_DAS) == 0)
		{
			if (strcmp(it->name(), MARKUP_TIMER) == 0)
				settings.DASTimer = parseTimer(*it, settings.DASTimer);
			else if (strcmp(it->name(), MARKUP_STEPTOENABLE) == 0)
				settings.stepToDAS = parseStepToEnable(*it, settings.stepToDAS);
		}
	}

	return true;
}

bool SettingsWrapper::parseVisual(const pugi::xml_node & visualNode)
{
	for (auto it = visualNode.begin(); it != visualNode.end(); ++it)
	{
		if (strcmp(it->name(), MARKUP_FLASH) == 0)
		{
			for (auto inner = it->begin(); inner != it->end(); ++inner)
			{
				if (strcmp(it->name(), MARKUP_TIMER) == 0)
					settings.flashAnimDelay = parseTimer(*inner, settings.flashAnimDelay);
				else if (strcmp(it->name(), MARKUP_COLOR) == 0)
				{
					if (strcmp(it->attribute(ATTR_ID).as_string(), ATTR_ID_A) == 0)
						settings.flashingSprite_color = parseColor(*inner, settings.flashingSprite_color);
					else
						settings.flashingSprite_altColor = parseColor(*inner, settings.flashingSprite_altColor);
				}
			}
		}

		else if (strcmp(it->name(), MARKUP_ACTIVE) == 0)
		{
			if (strcmp(it->name(), MARKUP_COLOR) == 0)
				settings.activeSprite_color = parseColor(*it, settings.activeSprite_color);
		}

		else if (strcmp(it->name(), MARKUP_LOCKED) == 0)
		{
			if (strcmp(it->name(), MARKUP_COLOR) == 0)
				settings.lockedSprite_color = parseColor(*it, settings.lockedSprite_color);
		}

		else if (strcmp(it->name(), MARKUP_GHOST) == 0)
		{
			if (strcmp(it->name(), MARKUP_COLOR) == 0)
				settings.ghostSprite_color = parseColor(*it, settings.ghostSprite_color);
		}
	}

	return true;
}

bool SettingsWrapper::parseScore(const pugi::xml_node & scoreNode)
{
	for (auto it = scoreNode.begin(); it != scoreNode.end(); ++it)
	{
		if (strcmp(it->name(), MARKUP_LINE) == 0)
		{
			for (auto inner = it->begin(); inner != it->end(); ++inner)
			{
				if (strcmp(inner->name(), MARKUP_SCORE) == 0)
					settings.lineScore = parseScore(*inner, settings.lineScore);
				else if (strcmp(inner->name(), MARKUP_MULTIPLIER) == 0)
					settings.lineMultiplier = parseMultiplier(*inner, settings.lineMultiplier);
			}
		}

		else if (strcmp(it->name(), MARKUP_LEVEL) == 0)
		{
			for (auto inner = it->begin(); inner != it->end(); ++inner)
			{
				if (strcmp(inner->name(), MARKUP_MULTIPLIER) == 0)
					settings.levelMultipler = parseMultiplier(*inner, settings.levelMultipler);
				else if (strcmp(inner->name(), MARKUP_SPEED) == 0)
					settings.levelSpeed = parseSpeed(*inner, settings.levelSpeed);
				else if (strcmp(inner->name(), MARKUP_LINE) == 0)
					settings.linesToLevelup = parseLine(*inner, settings.linesToLevelup);
			}
		}
	}

	return true;
}

bool SettingsWrapper::parseBoards(const pugi::xml_node & boardsNode)
{
	allocateBoards();

	size_t nextBoardIndex = 0;
	for (auto it = boardsNode.begin(); it != boardsNode.end(); ++it)
	{
		//Ignore everything that is not a <board>
		if (strcmp(it->name(), MARKUP_BOARD) != 0) continue;

		// Index Override
		size_t index;
		if (it->attribute(ATTR_INDEX)) index = it->attribute(ATTR_INDEX).as_int();
		else index = nextBoardIndex;

		//Check for valid index
		if (index >= settings.boardCount) continue;

		//Check if a board isn't already allocated here
		// if yes, swap
		if (settings.boardsArray[index] != nullptr) boardsSwap(index, nextBoardIndex);

		//Load the board
		settings.boardsArray[index] = allocateCustomBoard(*it);

		//Find the next empty index
		//Break if none is found.
		while (settings.boardsArray[nextBoardIndex] != nullptr)
		{
			if (++nextBoardIndex >= settings.boardCount) break;
		}
	}

	return true;
}

BoardSettings * SettingsWrapper::allocateCustomBoard(const pugi::xml_node & boardNode)
{
	BoardSettings* ret = new BoardSettings;
	resetBoard(*ret);

	for (auto it = boardNode.begin(); it != boardNode.end(); ++it)
	{
		if (strcmp(it->name(), MARKUP_NAME) == 0)
			ret->name = parseName(*it, ret->name);

		else if (strcmp(it->name(), MARKUP_RECORDING) == 0)
			ret->recording = true;

		else if (strcmp(it->name(), MARKUP_RECURRING) == 0)
			ret->isRecurring = true;

		else if (strcmp(it->name(), MARKUP_CONTROL) == 0)
		{
			for (auto inner = it->begin(); inner != it->end(); ++inner)
			{
				if (strcmp(inner->name(), MARKUP_AI) == 0) {/*Not implemented yet*/}
				else if (strcmp(inner->name(), MARKUP_LEARNING) == 0) 
				{
					if (!parseLearning(*inner, *ret))
					{
						delete ret;
						return nullptr;
					}
				}
				else if (strcmp(inner->name(), MARKUP_KEYBOARD) == 0)
				{
					if (!parseKeyboard(*inner, *ret))
					{
						delete ret;
						return nullptr;
					}
				}
			}
		}
	}

	return ret;
}

void SettingsWrapper::resetBoard(BoardSettings & board)
{
	using EnumArray = EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS>;

	board.name = DEFAULT_BOARD_NAME;
	board.control = DEFAULT_BOARD_CONTROL;
	board.recording = DEFAULT_RECORDING;
	board.isRecurring = DEFAULT_BOARD_RECURRING;

	EnumArray control;

	control[InputKeys::UP] = DEFAULT_KEY_UP;
	control[InputKeys::LEFT] = DEFAULT_KEY_LEFT;
	control[InputKeys::RIGHT] = DEFAULT_KEY_RIGHT;
	control[InputKeys::DOWN] = DEFAULT_KEY_DOWN;
	control[InputKeys::LFLIP] = DEFAULT_KEY_LFLIP;
	control[InputKeys::RFLIP] = DEFAULT_KEY_RFLIP;
	control[InputKeys::HOLD] = DEFAULT_KEY_HOLD;

	board.setUnion<EnumArray>(control);
}

bool SettingsWrapper::parseKeyboard(const pugi::xml_node& keyboardNode, BoardSettings & board)
{
	using EnumArray = EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS>;

	board.control = InputMethod::Player;

	for (auto it = keyboardNode.begin(); it != keyboardNode.end(); ++it)
	{
		//Ignore everything but <key>
		if (strcmp(it->name(), MARKUP_KEY) != 0) continue;

		if (strcmp(it->attribute(ATTR_INPUT).as_string(), ATTR_INPUT_UP) == 0)
			board.getUnion<EnumArray>()[InputKeys::UP] = parseKey(*it, board.getUnion<EnumArray>()[InputKeys::UP]);

		else if (strcmp(it->attribute(ATTR_INPUT).as_string(), ATTR_INPUT_DOWN) == 0)
			board.getUnion<EnumArray>()[InputKeys::DOWN] = parseKey(*it, board.getUnion<EnumArray>()[InputKeys::DOWN]);

		else if (strcmp(it->attribute(ATTR_INPUT).as_string(), ATTR_INPUT_LEFT) == 0)
			board.getUnion<EnumArray>()[InputKeys::LEFT] = parseKey(*it, board.getUnion<EnumArray>()[InputKeys::LEFT]);

		else if (strcmp(it->attribute(ATTR_INPUT).as_string(), ATTR_INPUT_RIGHT) == 0)
			board.getUnion<EnumArray>()[InputKeys::RIGHT] = parseKey(*it, board.getUnion<EnumArray>()[InputKeys::RIGHT]);

		else if (strcmp(it->attribute(ATTR_INPUT).as_string(), ATTR_INPUT_RFLIP) == 0)
			board.getUnion<EnumArray>()[InputKeys::LFLIP] = parseKey(*it, board.getUnion<EnumArray>()[InputKeys::LFLIP]);

		else if (strcmp(it->attribute(ATTR_INPUT).as_string(), ATTR_INPUT_LFLIP) == 0)
			board.getUnion<EnumArray>()[InputKeys::RFLIP] = parseKey(*it, board.getUnion<EnumArray>()[InputKeys::RFLIP]);

		else if (strcmp(it->attribute(ATTR_INPUT).as_string(), ATTR_INPUT_HOLD) == 0)
			board.getUnion<EnumArray>()[InputKeys::HOLD] = parseKey(*it, board.getUnion<EnumArray>()[InputKeys::HOLD]);
	}

	return true;
}

bool SettingsWrapper::parseLearning(const pugi::xml_node & learningNode, BoardSettings & board)
{
	board.control = InputMethod::Learning;

	for (auto it = learningNode.begin(); it != learningNode.end(); ++it)
	{
		//Ignore everything but <path>
		if (strcmp(it->name(), MARKUP_PATH) != 0) continue;
		else
		{
			board.setUnion<std::string>(parsePath(*it, DEFAULT_SCRIPT_PATH));
			return true;
		}
	}

	return false;
}

sf::Vector2i SettingsWrapper::parseSize(const pugi::xml_node & node, const sf::Vector2i & defaultValue) const
{
	return parseVector(node, defaultValue);
}

int SettingsWrapper::parseSize(const pugi::xml_node & node, const int defaultValue) const
{
	return parseInt(node, defaultValue);
}

sf::Vector2f SettingsWrapper::parsePosition(const pugi::xml_node & node, const sf::Vector2f & defaultValue) const
{
	return parseVector(node, defaultValue);
}

std::string SettingsWrapper::parseName(const pugi::xml_node & node, const std::string & defaultValue) const
{
	return parseString(node, defaultValue);
}

std::string SettingsWrapper::parsePath(const pugi::xml_node & node, const std::string & defaultValue) const
{
	return parseString(node, defaultValue);
}

int SettingsWrapper::parsePoolsize(const pugi::xml_node & node, const int defaultValue) const
{
	return parseInt(node, defaultValue);
}

std::string SettingsWrapper::parseTexture(const pugi::xml_node & node, const std::string & defaultValue) const
{
	return parseString(node, defaultValue);
}

std::string SettingsWrapper::parseCounter(const pugi::xml_node & node, const std::string & defaultValue) const
{
	return parseString(node, defaultValue);
}

int SettingsWrapper::parseLock(const pugi::xml_node & node, const int defaultValue) const
{
	return parseInt(node, defaultValue);
}

int SettingsWrapper::parseAutoScroll(const pugi::xml_node & node, const int defaultValue) const
{
	return parseInt(node, defaultValue);
}

int SettingsWrapper::parseFlash(const pugi::xml_node & node, const int defaultValue) const
{
	return parseInt(node, defaultValue);
}

int SettingsWrapper::parseTimer(const pugi::xml_node & node, const int defaultValue) const
{
	return parseInt(node, defaultValue);
}

int SettingsWrapper::parseStepToEnable(const pugi::xml_node & node, const int defaultValue) const
{
	return parseInt(node, defaultValue);
}

float SettingsWrapper::parseScore(const pugi::xml_node & node, const float defaultValue) const
{
	return parseFloat(node, defaultValue);
}

float SettingsWrapper::parseMultiplier(const pugi::xml_node & node, const float defaultValue) const
{
	return parseFloat(node, defaultValue);
}

float SettingsWrapper::parseSpeed(const pugi::xml_node & node, const float defaultValue) const
{
	return parseFloat(node, defaultValue);
}

float SettingsWrapper::parseLine(const pugi::xml_node & node, const float defaultValue) const
{
	return parseFloat(node, defaultValue);
}

void SettingsWrapper::boardsSwap(size_t indexA, size_t indexB)
{
	BoardSettings* tmp = settings.boardsArray[indexA];
	settings.boardsArray[indexA] = settings.boardsArray[indexB];
	settings.boardsArray[indexB] = tmp;
}

const GeneralSettings & SettingsWrapper::exposeData() const
{
	return settings;
}

sf::Keyboard::Key SettingsWrapper::parseKey(const pugi::xml_node& node, const sf::Keyboard::Key& defaultValue) const
{
	if (strcmp(node.child_value(), KEY_SQUARE) == 0)
		return defaultValue;
	if (strcmp(node.child_value(), KEY_1) == 0)
		return sf::Keyboard::Key::Num1;
	if (strcmp(node.child_value(), KEY_2) == 0)
		return sf::Keyboard::Key::Num2;
	if (strcmp(node.child_value(), KEY_3) == 0)
		return sf::Keyboard::Key::Num3;
	if (strcmp(node.child_value(), KEY_4) == 0)
		return sf::Keyboard::Key::Num4;
	if (strcmp(node.child_value(), KEY_5) == 0)
		return sf::Keyboard::Key::Num5;
	if (strcmp(node.child_value(), KEY_6) == 0)
		return sf::Keyboard::Key::Num6;
	if (strcmp(node.child_value(), KEY_7) == 0)
		return sf::Keyboard::Key::Num7;
	if (strcmp(node.child_value(), KEY_8) == 0)
		return sf::Keyboard::Key::Num8;
	if (strcmp(node.child_value(), KEY_9) == 0)
		return sf::Keyboard::Key::Num9;
	if (strcmp(node.child_value(), KEY_0) == 0)
		return sf::Keyboard::Key::Num0;
	if (strcmp(node.child_value(), KEY_DEGREE) == 0)
		return sf::Keyboard::Key::RBracket;
	if (strcmp(node.child_value(), KEY_PLUS) == 0)
		return sf::Keyboard::Key::Equal;
	if (strcmp(node.child_value(), KEY_BACKSPACE) == 0)
		return sf::Keyboard::Key::BackSpace;
	if (strcmp(node.child_value(), KEY_TAB) == 0)
		return sf::Keyboard::Key::Tab;
	if (strcmp(node.child_value(), KEY_A) == 0)
		return sf::Keyboard::Key::A;
	if (strcmp(node.child_value(), KEY_Z) == 0)
		return sf::Keyboard::Key::Z;
	if (strcmp(node.child_value(), KEY_E) == 0)
		return sf::Keyboard::Key::E;
	if (strcmp(node.child_value(), KEY_R) == 0)
		return sf::Keyboard::Key::R;
	if (strcmp(node.child_value(), KEY_T) == 0)
		return sf::Keyboard::Key::T;
	if (strcmp(node.child_value(), KEY_Y) == 0)
		return sf::Keyboard::Key::Y;
	if (strcmp(node.child_value(), KEY_U) == 0)
		return sf::Keyboard::Key::U;
	if (strcmp(node.child_value(), KEY_I) == 0)
		return sf::Keyboard::Key::I;
	if (strcmp(node.child_value(), KEY_O) == 0)
		return sf::Keyboard::Key::O;
	if (strcmp(node.child_value(), KEY_P) == 0)
		return sf::Keyboard::Key::P;
	if (strcmp(node.child_value(), KEY_HAT) == 0)
		return defaultValue;
	if (strcmp(node.child_value(), KEY_DOLLARS) == 0)
		return defaultValue;
	if (strcmp(node.child_value(), KEY_ENTER) == 0)
		return sf::Keyboard::Key::Return;
	if (strcmp(node.child_value(), KEY_VERRMAJ) == 0)
		return defaultValue;
	if (strcmp(node.child_value(), KEY_Q) == 0)
		return sf::Keyboard::Key::Q;
	if (strcmp(node.child_value(), KEY_S) == 0)
		return sf::Keyboard::Key::S;
	if (strcmp(node.child_value(), KEY_D) == 0)
		return sf::Keyboard::Key::D;
	if (strcmp(node.child_value(), KEY_F) == 0)
		return sf::Keyboard::Key::F;
	if (strcmp(node.child_value(), KEY_G) == 0)
		return sf::Keyboard::Key::G;
	if (strcmp(node.child_value(), KEY_H) == 0)
		return sf::Keyboard::Key::H;
	if (strcmp(node.child_value(), KEY_J) == 0)
		return sf::Keyboard::Key::J;
	if (strcmp(node.child_value(), KEY_K) == 0)
		return sf::Keyboard::Key::K;
	if (strcmp(node.child_value(), KEY_L) == 0)
		return sf::Keyboard::Key::L;
	if (strcmp(node.child_value(), KEY_M) == 0)
		return sf::Keyboard::Key::M;
	if (strcmp(node.child_value(), KEY_PERCENT) == 0)
		return defaultValue;
	if (strcmp(node.child_value(), KEY_MU) == 0)
		return defaultValue;
	if (strcmp(node.child_value(), KEY_MAJ) == 0)
		return sf::Keyboard::Key::LShift;
	if (strcmp(node.child_value(), KEY_DASH) == 0)
		return defaultValue;
	if (strcmp(node.child_value(), KEY_W) == 0)
		return sf::Keyboard::Key::W;
	if (strcmp(node.child_value(), KEY_X) == 0)
		return sf::Keyboard::Key::X;
	if (strcmp(node.child_value(), KEY_C) == 0)
		return sf::Keyboard::Key::C;
	if (strcmp(node.child_value(), KEY_V) == 0)
		return sf::Keyboard::Key::V;
	if (strcmp(node.child_value(), KEY_B) == 0)
		return sf::Keyboard::Key::B;
	if (strcmp(node.child_value(), KEY_N) == 0)
		return sf::Keyboard::Key::N;
	if (strcmp(node.child_value(), KEY_QUESTION) == 0)
		return sf::Keyboard::Key::Comma;
	if (strcmp(node.child_value(), KEY_DOT) == 0)
		return sf::Keyboard::Key::SemiColon;
	if (strcmp(node.child_value(), KEY_SLASH) == 0)
		return defaultValue;
	if (strcmp(node.child_value(), KEY_BANG) == 0)
		return defaultValue;
	if (strcmp(node.child_value(), KEY_RMAJ) == 0)
		return sf::Keyboard::Key::RShift;
	if (strcmp(node.child_value(), KEY_CTRL) == 0)
		return sf::Keyboard::Key::LControl;
	if (strcmp(node.child_value(), KEY_ALT) == 0)
		return sf::Keyboard::Key::LAlt;
	if (strcmp(node.child_value(), KEY_SPACE) == 0)
		return sf::Keyboard::Key::Space;
	if (strcmp(node.child_value(), KEY_RALT) == 0)
		return sf::Keyboard::Key::RAlt;
	if (strcmp(node.child_value(), KEY_RCTRL) == 0)
		return sf::Keyboard::Key::RControl;
	if (strcmp(node.child_value(), KEY_NUM_SLASH) == 0)
		return sf::Keyboard::Key::Slash;
	if (strcmp(node.child_value(), KEY_NUM_STAR) == 0)
		return sf::Keyboard::Key::Multiply;
	if (strcmp(node.child_value(), KEY_NUM_MINUS) == 0)
		return sf::Keyboard::Key::Subtract;
	if (strcmp(node.child_value(), KEY_NUM_7) == 0)
		return sf::Keyboard::Key::Numpad7;
	if (strcmp(node.child_value(), KEY_NUM_8) == 0)
		return sf::Keyboard::Key::Numpad8;
	if (strcmp(node.child_value(), KEY_NUM_9) == 0)
		return sf::Keyboard::Key::Numpad9;
	if (strcmp(node.child_value(), KEY_NUM_4) == 0)
		return sf::Keyboard::Key::Numpad4;
	if (strcmp(node.child_value(), KEY_NUM_5) == 0)
		return sf::Keyboard::Key::Numpad5;
	if (strcmp(node.child_value(), KEY_NUM_6) == 0)
		return sf::Keyboard::Key::Numpad6;
	if (strcmp(node.child_value(), KEY_NUM_1) == 0)
		return sf::Keyboard::Key::Numpad1;
	if (strcmp(node.child_value(), KEY_NUM_2) == 0)
		return sf::Keyboard::Key::Numpad2;
	if (strcmp(node.child_value(), KEY_NUM_3) == 0)
		return sf::Keyboard::Key::Numpad3;
	if (strcmp(node.child_value(), KEY_NUM_0) == 0)
		return sf::Keyboard::Key::Numpad0;
	if (strcmp(node.child_value(), KEY_NUM_PLUS) == 0)
		return sf::Keyboard::Key::Add;
	if (strcmp(node.child_value(), KEY_NUM_ENTER) == 0)
		return sf::Keyboard::Key::Return;
	if (strcmp(node.child_value(), KEY_NUM_DOT) == 0)
		return sf::Keyboard::Key::Delete;
	if (strcmp(node.child_value(), KEY_UP) == 0)
		return sf::Keyboard::Key::Up;
	if (strcmp(node.child_value(), KEY_DOWN) == 0)
		return sf::Keyboard::Key::Down;
	if (strcmp(node.child_value(), KEY_LEFT) == 0)
		return sf::Keyboard::Key::Left;
	if (strcmp(node.child_value(), KEY_RIGHT) == 0)
		return sf::Keyboard::Key::Right;

	return defaultValue;
}

int SettingsWrapper::parseInt(const pugi::xml_node & node, const int defaultValue) const
{
	return node.text().as_int(defaultValue);
}

float SettingsWrapper::parseFloat(const pugi::xml_node & node, const float defaultValue) const
{
	return node.text().as_float(defaultValue);
}

std::string SettingsWrapper::parseString(const pugi::xml_node & node, const std::string & defaultValue) const
{
	return std::string(node.text().as_string(defaultValue.c_str()));
}

sf::Color SettingsWrapper::parseColor(const pugi::xml_node & node, const sf::Color & defaultValue) const
{
	sf::Color ret = defaultValue;
	
	//Return default if no attribute type
	if (!node.attribute(ATTR_TYPE)) return ret;

	if (strcmp(node.attribute(ATTR_TYPE).as_string(), ATTR_TYPE_PLAIN) == 0)
	{
		if (strcmp(node.child_value(), COLOR_WHITE) == 0)
			ret = sf::Color::White;
		if (strcmp(node.child_value(), COLOR_BLUE) == 0)
			ret = sf::Color::Blue;
		if (strcmp(node.child_value(), COLOR_BLACK) == 0)
			ret = sf::Color::Black;
		if (strcmp(node.child_value(), COLOR_CYAN) == 0)
			ret =  sf::Color::Cyan;
		if (strcmp(node.child_value(), COLOR_MAGENTA) == 0)
			ret =  sf::Color::Magenta;
		if (strcmp(node.child_value(), COLOR_GREEN) == 0)
			ret =  sf::Color::Green;
		if (strcmp(node.child_value(), COLOR_RED) == 0)
			ret =  sf::Color::Red;
		if (strcmp(node.child_value(), COLOR_YELLOW) == 0)
			ret =  sf::Color::Yellow;
	}

	else if (strcmp(node.attribute(ATTR_TYPE).as_string(), ATTR_TYPE_RGB) == 0)
	{
		for (auto it = node.begin(); it != node.end(); ++it)
		{
			if (strcmp(it->name(), MARKUP_RED) == 0)
				ret.r = it->text().as_uint();
			else if (strcmp(it->name(), MARKUP_GREEN) == 0)
				ret.g = it->text().as_uint();
			else if (strcmp(it->name(), MARKUP_BLUE) == 0)
				ret.b = it->text().as_uint();
			else if (strcmp(it->name(), MARKUP_ALPHA) == 0)
				ret.a = it->text().as_uint();
		}
	}

	return ret;
}

sf::Vector2f SettingsWrapper::parseVector(const pugi::xml_node & node, const sf::Vector2f & defaultValue) const
{
	sf::Vector2f ret;

	for (auto it = node.begin(); it != node.end(); ++it)
	{
		if (strcmp(it->name(), MARKUP_WIDTH) == 0 || strcmp(it->name(), MARKUP_X) == 0)
			ret.x = it->text().as_float(defaultValue.x);
		else if (strcmp(it->name(), MARKUP_HEIGHT) == 0 || strcmp(it->name(), MARKUP_Y) == 0)
			ret.y = it->text().as_float(defaultValue.y);
	}

	return ret;
}

sf::Vector2i SettingsWrapper::parseVector(const pugi::xml_node & node, const sf::Vector2i & defaultValue) const
{
	return static_cast<sf::Vector2i>(parseVector(node, static_cast<sf::Vector2f>(defaultValue)));
}

char SettingsWrapper::parseChar(const pugi::xml_node & node, const char defaultValue) const
{
	return parseString(node, "" + defaultValue).at(0);
}
