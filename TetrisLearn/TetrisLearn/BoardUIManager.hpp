#pragma once

#include "SFML\Graphics.hpp"

#include "const.hpp"

#include "IBoardUI.hpp"



class TetraThumbnail;
class IBoard;

class BoardUIManager : public IBoardUI
{
public: 

	BoardUIManager(sf::Text& scoreCounter, sf::Text& levelCounter, sf::Text& gameOverText, TetraThumbnail& hold, TetraThumbnail* previews[THUMBNAIL_PREVIEW_NUMBER]);
	~BoardUIManager();

private:
	sf::Text& scoreCounter;
	sf::Text& levelCounter;
	sf::Text& gameOverText;

	TetraThumbnail& hold_thumbnail;
	TetraThumbnail* previews_thumbnails[THUMBNAIL_PREVIEW_NUMBER];


	// Inherited via IBoardUI
	virtual void changeScoreUI_custom(size_t newScore) override;
	virtual void changeLevelUI_custom(size_t newLevel) override;

	virtual void newHoldTetramino_custom(TetraminoTypes type) override;
	virtual void setThumbnail_custom(size_t index, TetraminoTypes type) override;

	virtual void gameOverUI_custom(bool isOver) override;
	
	
	// Ad-hoc
	sf::String stringPadding(size_t value, size_t stringSize);
};