#pragma once

#include <memory>

#include "SFML\Graphics.hpp"

#include "const.hpp"
#include "enum.hpp"

#include "IBoard.hpp"
#include "ITetraminoManager.hpp"
#include "TetrisTimer.hpp"
#include "ScoreManager.hpp"
#include "BoardUIManager.hpp"
#include "BoardGrid.hpp"
#include "TetraminoManager.hpp"
#include "InputData.hpp"

class IBlocksPool;
class TetraThumbnail;
class TetrisGame;


class Board : public IBoard
{
public:
	Board(sf::Text& scoreCounter, sf::Text& levelCounter, sf::Text& gameOverText, TetraThumbnail& hold, TetraThumbnail* previews[THUMBNAIL_PREVIEW_NUMBER],
		IBlocksPool& blockPool, TetrisGame& parent, const sf::Vector2f anchor, bool isRecording);

private:

	//State
	BoardStates state;
	void changeState(BoardStates newState, bool force = false);

	// Score and Levels.
	ScoreManager score;

	// Score / level tracking /and UI links
	BoardUIManager uiManager;

	//Tetramino
	TetraminoManager tetraminos;

	//Content
	BoardGrid content;

	// Timer Management
	// Simple pointer for ProtoEng compatibility.
	TetrisTimer* lockTimer;
	TetrisTimer* autoScrollTimer;
	TetrisTimer* flashTimer;

	//Block pool
	IBlocksPool& blockPool;

	//Parent
	TetrisGame& parent;

	//Log Move and reward for learning
	std::shared_ptr<InputData> current_move = nullptr;
	bool isRecording;

	// Inherited via IBoard
	virtual void act_custom() override;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	virtual void notifyGraphicUpdate_custom(sf::Time deltaTime) override;
	virtual void input_custom(uint32_t inputCode) override;

	virtual void changeScoreUI_custom(size_t newScore) override;
	virtual void changeLevelUI_custom(size_t newLevel) override;
	virtual void newHoldTetramino_custom(TetraminoTypes type) override;
	virtual void setThumbnail_custom(size_t index, TetraminoTypes type) override;
	virtual void gameOverUI_custom(bool isOver) override;

	virtual bool postLockRowChecks_custom() override;
	virtual void removeRows_custom() override;
	virtual Block ** bracket_custom(int i) override;
	virtual Block * const * bracket_custom(int i) const override;
	virtual bool isFree_custom(sf::Vector2i pos) const override;
	virtual Block * getBlock_custom(sf::Vector2i pos) const override;
	virtual void setBlock_custom(Block * content, sf::Vector2i pos) override;

	virtual void addScore_custom(size_t lineCleared) override;
	virtual size_t simulateScore_custom(size_t lineCleared, size_t level) const override;
	virtual size_t getScore_custom() const override;
	virtual size_t getLevel_custom() const override;

	virtual Block * getBlock_custom() override;
	virtual void storeBlock_custom(Block * block) override;

	virtual void gameOver_custom() override;

	virtual std::string getStrInputData_custom(InputKeys overrideInput) override;

	//Add-hoc
	void registerNewInput(Inputs input);
	void recordNewPosition();
	void recordNewScore(size_t linesCleared);
};