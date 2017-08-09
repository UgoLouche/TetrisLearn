#include "enum.hpp"

// Enum Functions
TetraminoTypes& operator++(TetraminoTypes& self, int)
{
	if (self == TetraminoTypes::Z) self = TetraminoTypes::I;
	else self = static_cast<TetraminoTypes>(static_cast<int>(self) + 1);

	return self;
}

// Some enum operator overload (obviously not complete).
uint32_t operator& (uint32_t a, Inputs b)
{
	return a & static_cast<uint32_t>(b);
}

uint32_t operator& (Inputs b, uint32_t a)
{
	return a & b;
}

uint32_t operator| (uint32_t a, Inputs b)
{
	return a | static_cast<uint32_t>(b);
}

uint32_t operator| (Inputs b, uint32_t a)
{
	return a | b;
}

InputKeys & operator++(InputKeys & self, int)
{
	if (self == InputKeys::HOLD) self = InputKeys::UP;
	else self = static_cast<InputKeys>(static_cast<int>(self) + 1);

	return self;
}
