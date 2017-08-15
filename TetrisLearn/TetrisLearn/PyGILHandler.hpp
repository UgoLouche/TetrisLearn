#pragma once

// Ugly hack to let python work without calling the debug dll...
// See https://bugs.python.org/issue22411
#ifdef _DEBUG
#undef _DEBUG
#include <Python/python.h>
#define _DEBUG
#else
#include <Python/python.h>
#endif


class PyGILHandler
{
public:
	PyGILHandler();
	~PyGILHandler();

private:
	PyGILState_STATE gstate;
};