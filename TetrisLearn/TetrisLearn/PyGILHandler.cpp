#include "PyGILHandler.hpp"


PyGILState_STATE PyGILHandler::gstate;

PyGILHandler::PyGILHandler()
{
}

PyGILHandler::~PyGILHandler()
{
}

void PyGILHandler::getLock()
{
	gstate = PyGILState_Ensure();
}

void PyGILHandler::releaseLock()
{
	PyGILState_Release(gstate);
}
