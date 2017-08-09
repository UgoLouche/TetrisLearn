#include "TetraminoMoves.hpp"

const EnumeratedArray<sf::Vector2i[TETRAMINO_SIZE], TetraminoTypes, TETRAMINO_TYPES> TetraminoMoves::Spawn_offsets =
{
	// Offsets
	// Anchor    Block 1   Block 2   Block 3
	{ { 0,  0 },{ 1,  0 },{ 2,  0 },{ -1,  0 } }, // I
	{ { 0,  0 },{ 1,  0 },{ -1, 1 },{ -1,  0 } }, // J
	{ { 0,  0 },{ 1,  0 },{ 1,  1 },{ -1,  0 } }, // L
	{ { 0,  0 },{ 1,  0 },{ 1,  1 },{  0,  1 } }, // O
	{ { 0,  0 },{ 1,  1 },{ 0,  1 },{ -1,  0 } }, // S
	{ { 0,  0 },{ 1,  0 },{ 0,  1 },{ -1,  0 } }, // T
	{ { 0,  0 },{ 1,  0 },{ 0,  1 },{ -1,  1 } }  // Z
};

const EnumeratedArray<sf::Vector2i, MoveDirections, MOVE_DIRECTIONS> TetraminoMoves::Move_offsets =
{
	{ 0,  1 }, // Top
	{ 1,  0 }, // Right
	{ 0, -1 }, // Down
	{-1,  0 }  // Left 
};

const EnumeratedArray<sf::Transform, TetraminoPhases, TETRAMINO_PHASES> TetraminoMoves::RotationMatrix =
{
	sf::Transform::Identity,

	sf::Transform //Left
	(
		 0, -1,  0,
		 1,  0,  0,
		 0,  0,  1
	),

	sf::Transform //Reverse.
	(
		-1,  0,  0,
		 0, -1,  0,
		 0,  0,  1
	),

	sf::Transform //Right.
	(
		 0,  1,  0,
		-1,  0,  0,
		 0,  0,  1
	)
};

const EnumeratedArray<sf::Vector2i[SRS_OFFSET_VARIANTS], TetraminoPhases, TETRAMINO_PHASES>  TetraminoMoves::SRS_offset_base =
{
	// Offsets.
	// Off 1      Off 2      Off 3      Off 4      Off 5 
	{ { 0,  0 }, { 0,  0 }, { 0,  0 }, { 0,  0 }, { 0,  0 } }, // Origin.
	{ { 0,  0 }, {-1,  0 }, {-1, -1 }, { 0,  2 }, {-1,  2 } }, // Left.
	{ { 0,  0 }, { 0,  0 }, { 0,  0 }, { 0,  0 }, { 0,  0 } }, // Inverse,
	{ { 0,  0 }, { 1,  0 }, { 1, -1 }, { 0,  2 }, { 1,  2 } }  // Right
};

const EnumeratedArray<sf::Vector2i[SRS_OFFSET_VARIANTS], TetraminoPhases, TETRAMINO_PHASES>  TetraminoMoves::SRS_offset_I =
{
	// Offsets.
	// Off 1      Off 2      Off 3      Off 4      Off 5 
	{ { 0,  0 }, {-1,  0 }, { 2,  0 }, {-1,  0 }, { 2,  0 } }, // Origin.
	{ { 0,  1 }, { 0,  1 }, { 0,  1 }, { 0, -1 }, { 0,  2 } }, // Left.
	{ {-1,  1 }, { 1,  1 }, {-2,  1 }, { 1,  0 }, {-2,  0 } }, // Inverse,
	{ {-1,  0 }, { 0,  0 }, { 0,  0 }, { 0,  1 }, { 0, -2 } }  // Right
};

const EnumeratedArray<sf::Vector2i[SRS_OFFSET_VARIANTS], TetraminoPhases, TETRAMINO_PHASES>  TetraminoMoves::SRS_offset_O =
{
	// Offsets.
	// Off 1      Off 2      Off 3      Off 4      Off 5 
	{ { 0,  0 }, { 0,  0 }, { 0,  0 }, { 0,  0 }, { 0,  0 } }, // Origin.
	{ {-1,  0 }, {-1,  0 }, {-1,  0 }, {-1,  0 }, {-1,  0 } }, // Left.
	{ {-1, -1 }, {-1, -1 }, {-1, -1 }, {-1, -1 }, {-1, -1 } }, // Inverse.
	{ { 0, -1 }, { 0, -1 }, { 0, -1 }, { 0, -1 }, { 0, -1 } }  // Right
};