#pragma once

#include <string>
#include <fstream>
#include <memory>

#include "SFML\Graphics.hpp"

#include "const.hpp"
#include "enum.hpp"

class ITetraminoManager;
class Tetramino;
class IBoardContent;
class InputData
{
public:
	// Static
	static void setFileStream(std::ofstream* file);


	InputData(
		ITetraminoManager& tetraManager,
		IBoardContent& content,
		Inputs input,
		size_t previousID,
		bool isRecording
		);

	//Prevent copy
	InputData(const InputData& copy) = delete;
	InputData operator=(const InputData& copy) = delete;

	~InputData();

	// These fields can (and should) be modified after creation
	void setScore(size_t linesCleared, size_t normalizedScore);
	void setNewPosition(Tetramino& updatedTetramino);
	
	//String conversion
	std::string toString(bool inputOverride = false, InputKeys newInput = InputKeys::DOWN) const;

	size_t getID() const;

private:
	//Delte behaviour
	bool isRecording;

	// Static
	// init at 1, 0 is reserved for no predecessor.
	static size_t nextID;
	static std::ofstream* fileStream;
	size_t flushWatchdog = FLUSH_TIMER; //Ensure regular flushing


	TetraminoTypes currentTetra;
	TetraminoTypes thumbnails[THUMBNAIL_PREVIEW_NUMBER];
	TetraminoTypes hold;
	bool isHoldEmpty;
	
	//Board info
	bool boardContent[BOARD_VISIBLE_HEIGHT * BOARD_WIDTH];

	//Input Info
	InputKeys input;

	// Coordinate of each blocks of current_tetramino, before and after the move is processed
	sf::Vector2i coord_current[TETRAMINO_SIZE];
	sf::Vector2i coord_post[TETRAMINO_SIZE];

	TetraminoPhases phase;

	// Number of line cleared by the move and score incurred (without level multiplier though).
	size_t linesCleared = 0;
	size_t normalizedScore = 0;

	// Current move ID, and ID of the previous move
	size_t ID;
	size_t previousID;

	////// Methods


	//Sub routine for string conversion
	std::string convertTetraType(TetraminoTypes type, bool none = false) const;
	std::string convertTetraPhase(TetraminoPhases phase, bool none = false) const;
	std::string convertVector2(sf::Vector2i vector) const;
	std::string convertInput(InputKeys input, bool none = false) const;
	std::string convertBool(bool b) const;

	std::string unitVector(size_t totalDim, size_t onePosition, bool allZero = false) const;

};