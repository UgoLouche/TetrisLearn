#include "InputData.hpp"

#include "Settings.hpp"

#include "ITetraminoManager.hpp"
#include "Tetramino.hpp"
#include "IBoardContent.hpp"


#include <algorithm>

// ID 0 denote data without parent
size_t InputData::nextID = 1;

std::ofstream* InputData::fileStream = nullptr;

void InputData::setFileStream(std::ofstream * file)
{
	if (fileStream == nullptr)
	{
		fileStream = file;
	}
}

// For now copy all data, as this should avoid inconsistencies (i.e when move result in line supression, the board may change between instance creation
// and it destruction).
// If this is too much on CPU, you may want to only keep references to the board and expand on that only when written to file.
InputData::InputData(ITetraminoManager & tetraManager, IBoardContent& content, Inputs input, size_t previousID, bool isRecording) :
	currentTetra(tetraManager.current()->getType()),
	hold(tetraManager.getHoldType()),
	isHoldEmpty( !tetraManager.isHolding()),
	phase(tetraManager.current()->getPhase()),
	ID(nextID++),
	previousID(previousID),
	isRecording(isRecording)
{
	for (int i = 0; i < THUMBNAIL_PREVIEW_NUMBER; ++i) thumbnails[i] = tetraManager.getNextType(i);

	// Copy board content
	int offset;
	for (int i = 0; i < BOARD_WIDTH; ++i)
	{
		offset = i * BOARD_WIDTH;
		for (int j = 0; j < BOARD_VISIBLE_HEIGHT; ++j)
		{
			// This is stored in a weird order
			// (3 )(7 )(11)
			// (2 )(6 )(10)
			// (1 )(5 )(9 )
			// (0 )(4 )(8 )
			boardContent[offset + j] = ( content.getBlock(sf::Vector2i(i, j)) != nullptr );
		}
	}

	for (int i = 0; i < TETRAMINO_SIZE; ++i)
	{
		coord_current[i] = tetraManager.current()->getBlockBoardPosition(i);
		coord_post[i] = coord_current[i];
	}

	//Input conversion (because I'm bad and/or I like duplicating enums...)
	// TODO fix this duplicate enum thing
	switch (input)
	{
	case Inputs::Down:
		this->input = InputKeys::DOWN;
		break;
	case Inputs::Left:
		this->input = InputKeys::LEFT;
		break;
	case Inputs::Right:
		this->input = InputKeys::RIGHT;
		break;
	case Inputs::FlipR:
		this->input = InputKeys::RFLIP;
		break;
	case Inputs::FlipL:
		this->input = InputKeys::LFLIP;
		break;
	case Inputs::Hold:
		this->input = InputKeys::HOLD;
		break;
	case Inputs::DashDown:
		this->input = InputKeys::UP;
		break;
	// Mappedto nothing
	case Inputs::DashLeft:
	case Inputs::DashRight:
	default:
		this->input = InputKeys::DOWN;
		break;
	}
}

// Be extra careful here with copies
// Actually, copy constructor is deleted
InputData::~InputData()
{
	if (fileStream != nullptr && fileStream->is_open() && isRecording)
	{
		*fileStream << toString() + "\n";

		//Remember to flush regularly.
		if (--flushWatchdog == 0)
		{
			flushWatchdog = FLUSH_TIMER;
			fileStream->flush();
		}
	}
}

void InputData::setScore(size_t linesCleared, size_t normalizedScore)
{
	this->linesCleared = linesCleared;
	this->normalizedScore = normalizedScore;
}

void InputData::setNewPosition(Tetramino & updatedTetramino)
{
	for (int i = 0; i < TETRAMINO_SIZE; ++i) coord_post[i] = updatedTetramino.getBlockBoardPosition(i);
}

size_t InputData::getID() const
{
	return ID;
}

std::string InputData::toString(bool inputOverride, InputKeys newInput) const
{
	char sep = Settings::get().value_separator; //Shorthand
	std::string ret = "";

	//Current Tetra 0-6
	ret += convertTetraType(currentTetra) + sep;

	//Next Tetra 7-13 / 14-20 / 21-27
	for (int i = 0; i < THUMBNAIL_PREVIEW_NUMBER; ++i) ret += convertTetraType(thumbnails[i]) + sep;

	//Hold type 28-34
	ret += convertTetraType(hold, isHoldEmpty) + sep;

	// Board content 35-234
	for (int i = 0; i < BOARD_VISIBLE_HEIGHT * BOARD_WIDTH; ++i) ret += convertBool(boardContent[i]) + sep;

	//Input 235-241
	if (!inputOverride) ret += convertInput(input) + sep;
	else ret += convertInput(newInput) + sep;

	// Current Tetra block coordinate 242-243 / 244-245 / 246-247 / 248-249
	for (int i = 0; i < TETRAMINO_SIZE; ++i) ret += convertVector2(coord_current[i]) + sep;

	// Tetra block coordinate after input 250-251 / 252-253 / 254-255 / 256-257
	for (int i = 0; i < TETRAMINO_SIZE; ++i) ret += convertVector2(coord_post[i]) + sep;

	// Tetramino phase (before move) 258-261
	ret += convertTetraPhase(phase) + sep;

	// Line cleared 262
	ret += std::to_string(linesCleared) + sep;

	// Normalized score 263
	ret += std::to_string(normalizedScore) + sep;

	// previous move's ID 264
	ret += std::to_string(previousID) + sep;

	// move's ID 265
	ret += std::to_string(ID);

	return ret;
}

std::string InputData::convertTetraType(TetraminoTypes type, bool none) const
{
	return unitVector(TETRAMINO_TYPES, (size_t)type, none);
}

std::string InputData::convertTetraPhase(TetraminoPhases phase, bool none) const
{
	return unitVector(TETRAMINO_PHASES, (size_t)phase, none);
}

std::string InputData::convertVector2(sf::Vector2i vector) const
{
	return std::to_string(vector.x) + Settings::get().value_separator + std::to_string(vector.y);
}

std::string InputData::convertInput(InputKeys input, bool none) const
{
	return unitVector(TOTAL_KEYS, (size_t)input, none);
}

std::string InputData::convertBool(bool b) const
{
	return b ? "1" : "0";
}

std::string InputData::unitVector(size_t totalDim, size_t onePosition, bool allZero) const
{
	std::string ret = "";

	for (size_t i = 0; i < totalDim; ++i)
	{
		//Add comma betwwen loop
		if (i != 0) ret += Settings::get().value_separator;

		if (!allZero && i == onePosition) ret += "1";
		else ret += "0";
	}

	return ret;
}



