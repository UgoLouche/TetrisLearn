#include "PythonWrapper.hpp"

#include "const.hpp"

long PythonWrapper::instanceCounter = 0;

PyObject* PythonWrapper::getFunc = nullptr;

PythonWrapper::PythonWrapper(std::string className)
{
	//FIrst instance init routine
	if (instanceCounter == LONG_MAX) 
		throw new std::exception("Too many PythonWrapper");

	if (instanceCounter++ == 0 && pythonInit() == false)
		throw new std::exception("Python failed to init");


	GILHandler.getLock();
	//Get an instance of the estimator in className.py wrapped in Wrapper.py.
	PyObject* pArg = Py_BuildValue("s", className.c_str());

	if (pArg == NULL)
		throw new std::exception("Failed to convert file name");

	pInstance = PyObject_CallFunctionObjArgs(getFunc, pArg, NULL);

	if (pInstance == NULL)
		throw new std::exception("Failed to load estimator");

	Py_DECREF(pArg);

	GILHandler.releaseLock();
}

PythonWrapper::~PythonWrapper()
{
	if (learningThread != nullptr)
	{
		stopLearning();

		learningThread->join();
		delete(learningThread);
	}

	Py_DECREF(pInstance);

	if (--instanceCounter == 0)
		pythonFinal();
}

float PythonWrapper::scoreMove(std::string inputStr)
{
	float ret;
	PyObject *pFuncName, *pReturn, *pArgs;

	GILHandler.getLock();

	pFuncName = Py_BuildValue("s", PYTHON_PRED_METHOD);	
	if (pFuncName == NULL)
	{
		fprintf(stderr, "Error in scoreMove, function name\n");
		Py_DECREF(pFuncName);

		GILHandler.releaseLock();
		return -1;
	}

	pArgs = Py_BuildValue("s", inputStr.c_str());
	if (pArgs == NULL)
	{
		fprintf(stderr, "Error in scoreMove, Arg string\n");
		Py_DECREF(pArgs);

		GILHandler.releaseLock();
		return -1;
	}

	pReturn = PyObject_CallMethodObjArgs(pInstance, pFuncName, pArgs, NULL);
	Py_DECREF(pFuncName);
	Py_DECREF(pArgs);

	if (pReturn == NULL)
	{
		fprintf(stderr, "Error in scoreMove, function call\n");
		Py_DECREF(pReturn);

		GILHandler.releaseLock();
		return -1;
	}

	ret = (float)PyFloat_AsDouble(pReturn);

	Py_DECREF(pReturn);

	GILHandler.releaseLock();
	return ret;
}

void PythonWrapper::startLearning()
{
	if (isLearningFlag) return;

	isLearningFlag = true;
	keepLearning.test_and_set();

	learningThread = new std::thread( [this](){ this->learning(); } );
}

void PythonWrapper::stopLearning()
{
	keepLearning.clear();
}

bool PythonWrapper::isLearning()
{
	return isLearningFlag;
}

bool PythonWrapper::pythonInit()
{
	//Init interpreter
	Py_Initialize();

	PyObject *pName, *wrapperModule;

	//Load module
	pName = PyUnicode_DecodeFSDefault(PYTHON_MODULE); // Error checking of pName left out
	wrapperModule = PyImport_Import(pName);
	Py_DECREF(pName);

	//Check before next step
	if (wrapperModule == NULL)
	{
		fprintf(stderr, "Failed to load \"%s\"\n", PYTHON_MODULE);
		Py_DECREF(wrapperModule);
		return false;
	}

	//Load the getter Function
	getFunc = PyObject_GetAttrString(wrapperModule, PYTHON_FUNC);
	Py_DECREF(wrapperModule);

	//Check everything is fine
	if (getFunc == NULL || PyCallable_Check(getFunc) == 0)
	{
		fprintf(stderr, "Cannot find function \"%s\"\n", PYTHON_FUNC);
		Py_DECREF(getFunc);
		return false;
	}

	return true;
}

bool PythonWrapper::pythonFinal()
{
	Py_XDECREF(getFunc);

	//Apparently unstable with threading, cause access violation and seems optional
	//Commenting it out for now
	/*
	if (Py_FinalizeEx() < 0)
	{
		fprintf(stderr, "Error on Python Finalize\n");
		return false;
	}
	*/

	return true;
}

void PythonWrapper::learning()
{
	PyObject *pFuncName;

	GILHandler.getLock();

	pFuncName = Py_BuildValue("s", PYTHON_FIT_METHOD);
	if (pFuncName == NULL)
	{
		fprintf(stderr, "Error in startLearning, function name\n");
		Py_DECREF(pFuncName);

		GILHandler.releaseLock();
		return;
	}

	//Loop learning until flagged to stop
	while (keepLearning.test_and_set())
	{
		PyObject_CallMethodObjArgs(pInstance, pFuncName, NULL);
	}
	keepLearning.clear();

	Py_DECREF(pFuncName);
	GILHandler.releaseLock();
	
	isLearningFlag = false;
}
