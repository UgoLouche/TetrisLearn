#include "Controller.hpp"

Controller::Controller(IControlledObject& controlled, TetrisGame& parent) :
	controlled(controlled),
	parent(parent)
{}

Controller::~Controller()
{
}

