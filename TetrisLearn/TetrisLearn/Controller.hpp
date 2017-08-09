#pragma once

#include "IControlledObject.hpp"
#include "IActingObject.hpp"
#include "TetrisGame.hpp"

class Controller : public IActingObject
{
public:
	Controller(IControlledObject& controlled, TetrisGame& parent);
	virtual ~Controller();

protected:
	IControlledObject& controlled;
	TetrisGame& parent;
};