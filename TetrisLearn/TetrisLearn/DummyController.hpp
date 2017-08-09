#pragma once

#include "KeyboardController.hpp"
#include "IControlledObject.hpp"

/*
* Dummy, non-configurable controller
*/
class DummyController : public KeyboardController
{
public:
	DummyController(IControlledObject& controlled, TetrisGame& parent);
};