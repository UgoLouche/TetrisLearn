#include "BoardUIManager.hpp"

#include "TetraThumbnail.hpp"
#include "Settings.hpp"


BoardUIManager::BoardUIManager(sf::Text & scoreCounter, sf::Text & levelCounter, sf::Text & gameOverText, TetraThumbnail & hold, TetraThumbnail * previews[THUMBNAIL_PREVIEW_NUMBER]) :
	scoreCounter(scoreCounter),
	levelCounter(levelCounter),
	gameOverText(gameOverText),
	hold_thumbnail(hold)
{
	for (int i = 0; i < THUMBNAIL_PREVIEW_NUMBER; ++i) previews_thumbnails[i] = previews[i];
}

BoardUIManager::~BoardUIManager()
{
	for (int i = 0; i < THUMBNAIL_PREVIEW_NUMBER; ++i) previews_thumbnails[i]->destroyLater();
}

sf::String BoardUIManager::stringPadding(size_t value, size_t stringSize)
{
	std::string value_str = std::to_string(value);

	if (value_str.length() > stringSize) value_str = value_str.substr(value_str.length() - stringSize, stringSize);
	else
	{
		while (value_str.length() < stringSize) value_str = '0' + value_str;
	}

	return sf::String(value_str);
}

void BoardUIManager::changeScoreUI_custom(size_t newScore)
{
	scoreCounter.setString(stringPadding(newScore, SCORE_COUNTER_LENGTH));
}

void BoardUIManager::changeLevelUI_custom(size_t newLevel)
{
	levelCounter.setString(stringPadding(newLevel, LEVEL_COUNTER_LENGTH));
}

void BoardUIManager::newHoldTetramino_custom(TetraminoTypes type)
{
	hold_thumbnail.changeType(type);
}

void BoardUIManager::setThumbnail_custom(size_t index, TetraminoTypes type)
{
	previews_thumbnails[index]->changeType(type);
}

void BoardUIManager::gameOverUI_custom(bool isOver)
{
	gameOverText.setString(Settings::get().gameOver_string);
}
