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

#include <string>
#include <climits>
#include <atomic>
#include <thread>

#include "PyGILHandler.hpp"

class PythonWrapper
{
public:
	PythonWrapper(std::string className);
	~PythonWrapper();

	//Add arguments laters
	float scoreMove(std::string inputStr);


	//TODO Placeholder for now, need Implementation
	void startLearning();
	void stopLearning();
	bool isLearning();


private:
	//Interpreter init / finalization -- STATIC
	static bool pythonInit();
	static bool pythonFinal();

	static long instanceCounter;
	static PyObject* getFunc;

	//Python MThread support
	static PyThreadState* tstate;
	//PyGILHandler GILHandler = PyGILHandler();



	//Learning and Thread
	std::atomic_flag keepLearning;
	std::thread* learningThread = nullptr;

	bool isLearningFlag = false;
	void learning();

	//Non Static
	PyObject* pInstance;


};