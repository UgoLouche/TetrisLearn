#include "PyGILHandler.hpp"


//PyGILState_STATE PyGILHandler::gstate;

PyGILHandler::PyGILHandler()
{
	gstate = PyGILState_Ensure();
}

PyGILHandler::~PyGILHandler()
{
	PyGILState_Release(gstate);
}