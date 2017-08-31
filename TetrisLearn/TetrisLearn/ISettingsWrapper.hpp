#pragma once

#include <string>

#include "SFML\Graphics.hpp"

#include "enum.hpp"
#include "EnumeratedArray.hpp"
#include "const.hpp"
#include "BoardSettings.hpp"

typedef struct GeneralSettings
{
	//Window
	sf::Vector2i            winSize;
	std::string             winName;

	//Boards Grid
	sf::Vector2i            gridSize;

	//Texts - general
	sf::Color               textColor;
	sf::Vector2f            textPosition;
	std::string             textString;
	int                     textSize;

	//Counters/GameOver
	std::string             scoreText_string;
	std::string             scoreCounter_string;
	std::string             levelText_string;
	std::string             levelCounter_string;

	std::string             gameOver_string;
	sf::Color               gameOver_color;

	//Thumbnails
	sf::Color               holdThumbnail_color;
	sf::Color               previewsThumbnails_color;
			              
	std::string             holdThumnail_string;
	std::string             previewThumbnails_string;
			              
	int                     thumbnails_fontSize;

	//Board Name 
	int                     boardName_fontSize; //TODO generate default for that

	//Font
	std::string             fontPath;
	int                     fontSize;

	//Rules
	int                     lockTime;
	int                     autoscollTime;
	int                     flashTime;
	int                     DASTimer;
	int                     stepToDAS;

	//Visual effects
	int                     flashAnimDelay;
	sf::Color               activeSprite_color;
	sf::Color               lockedSprite_color;
	sf::Color               flashingSprite_color;
	sf::Color               flashingSprite_altColor;
	sf::Color               ghostSprite_color;

	//Block Management
	int                     blockPoolSize;
	std::string             blockTexturePath;

	//Scoring
	float                   lineScore;
	float                   lineMultiplier;
	float                   levelMultipler;
	float                   levelSpeed;
	float                   linesToLevelup;

	//File Recording
	//Not yet implemented
	//TODO implement this for config file
	char                   value_separator;
	std::string            save_path;

	//Board specific
	// boardArray is an array of pointer representing all boards.
	// In boardArray, a nullptr pointer means the corresponding board
	// has default setting and thus defaultBoard's value should be used.
	// 
	// getBoard(size_t) automaticly return the correct BoardSettings (either
	// defaultBoard or the one in boardsArray) depending of this.
	BoardSettings defaultBoard;
	BoardSettings** boardsArray;
	size_t boardCount;

}GeneralSettings;

class ISettingsWrapper
{
public:
	virtual ~ISettingsWrapper();

	//Accessors
	const GeneralSettings& getSettings() const;

	// See comment in GeneralSettings (board section) for details.
	const BoardSettings& getBoard(size_t index) const;

private:
	// Save time, only one pure virtual exposing all the data, 
	//and accessor are hardcoded into the interface.
	virtual const GeneralSettings& exposeData() const = 0;


};