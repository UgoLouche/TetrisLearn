#include "IControlledObject.hpp"

#include "enum.hpp"


IControlledObject::~IControlledObject()
{
}

void IControlledObject::inputDown()
{
	input((uint32_t)Inputs::Down);
}

void IControlledObject::inputLeft()
{
	input((uint32_t)Inputs::Left);
}

void IControlledObject::inputRight()
{
	input((uint32_t)Inputs::Right);
}

void IControlledObject::inputFlipR()
{
	input((uint32_t)Inputs::FlipR);
}

void IControlledObject::inputFlipL()
{
	input((uint32_t)Inputs::FlipL);
}

void IControlledObject::inputHold()
{
	input((uint32_t)Inputs::Hold);
}

void IControlledObject::inputDashLeft()
{
	input((uint32_t)Inputs::DashLeft);
}

void IControlledObject::inputDashRight()
{
	input((uint32_t)Inputs::DashRight);
}

void IControlledObject::inputDashDown()
{
	input((uint32_t)Inputs::DashDown);
}

void IControlledObject::input(uint32_t inputCode)
{
	input_custom(inputCode);
}
