#pragma once

#include <cstdint>

// Layers.
enum LayerType : int
{
	GameUI = 0,
	GameEntity,
	GameMessage
};


// Board.
enum class BoardStates
{
	Play,
	WaitForLock,
	Flash,
	Over
};


// Tetraminos.
enum class TetraminoTypes : int
{
	I = 0,
	J,
	L,
	O,
	S,
	T,
	Z
};
TetraminoTypes& operator++(TetraminoTypes& self, int);

enum class MoveDirections : int
{
	TOP = 0,
	RIGHT,
	DOWN,
	LEFT
};

enum class TetraminoPhases
{
	ORIGIN = 0,
	LEFT,
	INVERSE,
	RIGHT
};


//Blocks
enum class BlockStatus
{
	Active,
	Locked,
	Flashing,
	Inactive,
	Ghost
};


// Contollers
enum class Inputs : uint32_t
{
	Down = 1 << 1,
	Left = 1 << 2,
	Right = 1 << 3,
	FlipR = 1 << 4,
	FlipL = 1 << 5,
	Hold = 1 << 6,
	DashLeft = 1 << 7,
	DashRight = 1 << 8,
	DashDown = 1 << 9
};

uint32_t operator&(uint32_t a, Inputs b);
uint32_t operator&(Inputs a, uint32_t b);
uint32_t operator|(uint32_t a, Inputs b);
uint32_t operator|(Inputs a, uint32_t b);


// Keyboards
enum class InputKeys : int
{
	UP = 0,
	LEFT,
	RIGHT,
	DOWN,
	LFLIP,
	RFLIP,
	HOLD
};
InputKeys& operator++(InputKeys& self, int);


// Settings
enum class InputMethod
{
	Player,
	Static_AI,
	Learning
};
