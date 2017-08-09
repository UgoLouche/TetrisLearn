#pragma once

#include "SFML\Graphics.hpp"

#include "const.hpp"

#include "EnumeratedArray.hpp"

enum class TetraminoTypes;
enum class TetraminoPhases;
enum class MoveDirections;

// TODO maybe cleanup the enums a bit and put them all in a single file

struct TetraminoMoves
{
	// This is a 7 X 4 X 2 tensor. Everything can be initialized with
	// initialization list, hence the matrix notation below (where the last third mode is noted inline)
	static const EnumeratedArray<sf::Vector2i[TETRAMINO_SIZE], TetraminoTypes, TETRAMINO_TYPES> Spawn_offsets;

	static const EnumeratedArray<sf::Vector2i, MoveDirections, MOVE_DIRECTIONS> Move_offsets;

	// This is guideline SRS internal offset for wall-kick moves.
	// See https://tetris.wiki/SRS
	// I and O have special sets of offsets.
	static const EnumeratedArray<sf::Vector2i[SRS_OFFSET_VARIANTS], TetraminoPhases, TETRAMINO_PHASES> SRS_offset_base;
	static const EnumeratedArray<sf::Vector2i[SRS_OFFSET_VARIANTS], TetraminoPhases, TETRAMINO_PHASES> SRS_offset_I;
	static const EnumeratedArray<sf::Vector2i[SRS_OFFSET_VARIANTS], TetraminoPhases, TETRAMINO_PHASES> SRS_offset_O;

	// Could avoid specialization and directly use somthing (actually, maybe not because of initializer lists)
	// static const EnumeratedArray<EnumeratedArray<sf::Vector2i, int, SRS_OFFSET_VARIANTS>, TetraminoPhases, TETRAMINO_PHASES> foo;

	// Rotation.
	static const EnumeratedArray<sf::Transform, TetraminoPhases, TETRAMINO_PHASES> RotationMatrix;
};