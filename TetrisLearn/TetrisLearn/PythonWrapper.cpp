#include "PythonWrapper.hpp"

#include "const.hpp"

long PythonWrapper::instanceCounter = 0;

PyObject* PythonWrapper::getFunc = nullptr;

PyThreadState* PythonWrapper::tstate = nullptr;

PythonWrapper::PythonWrapper(std::string className)
{

	//FIrst instance init routine
	if (instanceCounter == LONG_MAX) 
		throw new std::exception("Too many PythonWrapper");

	if (instanceCounter++ == 0 && pythonInit() == false)
		throw new std::exception("Python failed to init");

	PyGILHandler GILHandler; //We need the GIL from here

	//Get an instance of the estimator in className.py wrapped in Wrapper.py.
	PyObject* pArg = Py_BuildValue("s", className.c_str());

	if (pArg == NULL)
		throw new std::exception("Failed to convert file name");

	pInstance = PyObject_CallFunctionObjArgs(getFunc, pArg, NULL);

	if (pInstance == NULL)
		throw new std::exception("Failed to load estimator");

	Py_DECREF(pArg);
}

PythonWrapper::~PythonWrapper()
{
	if (learningThread != nullptr)
	{
		stopLearning();

		learningThread->join();
		delete(learningThread);
	}

	//Need the GIL for one line...
	PyGILState_STATE gstate = PyGILState_Ensure();
	Py_DECREF(pInstance);
	PyGILState_Release(gstate);


	if (--instanceCounter == 0)
	{
		pythonFinal();
	}
}

float PythonWrapper::scoreMove(std::string inputStr)
{

	float ret;
	PyObject *pFuncName, *pReturn, *pArgs;
	
	PyGILHandler GILHandler; //Get the GIL

	pFuncName = Py_BuildValue("s", PYTHON_PRED_METHOD);
	if (pFuncName == NULL)
	{
		fprintf(stderr, "Error in scoreMove, function name\n");
		Py_DECREF(pFuncName);

		return -1;
	}

	pArgs = Py_BuildValue("s", inputStr.c_str());
	if (pArgs == NULL)
	{
		fprintf(stderr, "Error in scoreMove, Arg string\n");
		Py_DECREF(pArgs);

		return -1;
	}

	pReturn = PyObject_CallMethodObjArgs(pInstance, pFuncName, pArgs, NULL);
	Py_DECREF(pFuncName);
	Py_DECREF(pArgs);

	if (pReturn == NULL)
	{
		fprintf(stderr, "Error in scoreMove, function call\n");
		Py_DECREF(pReturn);

		return -1;
	}

	ret = (float)PyFloat_AsDouble(pReturn);
	

	Py_DECREF(pReturn);

	return ret;
}

void PythonWrapper::startLearning()
{
	if (isLearningFlag) return;

	isLearningFlag = true;
	keepLearning.test_and_set();

	learningThread = new std::thread([this]() { this->learning(); });
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
	PyEval_InitThreads(); //We hold the GIL from here

	//Import path and append custom script location
	PyObject *sys, *path, *newEntry;

	sys = PyImport_ImportModule("sys");
	if (sys == NULL)
	{
		fprintf(stderr, "Failed to load \"%s\"\n", "sys");
		Py_DECREF(sys);
		return false;
	}

	path = PyObject_GetAttrString(sys, "path");
	if (path == NULL)
	{
		fprintf(stderr, "Failed to find Attr \"%s\"\n", "path");
		Py_DECREF(path);
		return false;
	}

	newEntry = Py_BuildValue("s", "E:/Documents/Projects/TetrisLearn/TetrisLearn/TetrisLearn/PythonSrc"); //TODO Make this configurable.
	if (newEntry == NULL)
	{
		fprintf(stderr, "Failed to build string for Path\n");
		Py_DECREF(newEntry);
		return false;
	}

	PyList_Append(path, newEntry);
	
	Py_DECREF(newEntry);
	Py_DECREF(path);
	Py_DECREF(sys);

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

		//Init is done, release GIL from Main thread
		tstate = PyEval_SaveThread();

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

		//Init is done, release GIL from Main thread
		tstate = PyEval_SaveThread();

		return false;
	}

	//Init is done, release GIL from Main thread
	tstate = PyEval_SaveThread();

	return true;
}

bool PythonWrapper::pythonFinal()
{
	//Get the GIL before closing the interpreter
	PyEval_RestoreThread(tstate);

	Py_XDECREF(getFunc);

	//Apparently unstable with threading, cause access violation and seems optional
	//Commenting it out for now
	
	Py_Finalize();
	

	return true;
}

void PythonWrapper::learning()
{

	PyObject *pFuncName, *pDataFile;

	PyGILHandler GILHandler; //Get the GIL (released on exit)
	
	pFuncName = Py_BuildValue("s", PYTHON_FIT_METHOD/*"fit"*/);
	if (pFuncName == NULL)
	{
		fprintf(stderr, "Error in startLearning, function name\n");
		Py_DECREF(pFuncName);

		return;
	}

	pDataFile = Py_BuildValue("s", "inputData_save.raw"); //TODO put this into setting
	if (pDataFile == NULL)
	{
		fprintf(stderr, "Error in startLearning, data file name\n");
		Py_DECREF(pDataFile);

		return;
	}

	//Loop learning until flagged to stop
	PyObject *pCallReturn = NULL; //See note at the end of the file.
	while (keepLearning.test_and_set())
	{
		//Store the return value, serve as a reminder that it does return something 
		//(and that it MUST return something, otherwise python fails to close properly at the end of the program (more in comment at the file's end)
		pCallReturn = PyObject_CallMethodObjArgs(pInstance, pFuncName, pDataFile, NULL); 

		if (pCallReturn == NULL)
		{
			fprintf(stderr, "Something went wrong\n");
			Py_DECREF(pFuncName);
			Py_DECREF(pDataFile);

			throw new std::exception("Error in learning");
		}

		//Py_DECREF(pCallReturn);
	}
	keepLearning.clear();

	Py_DECREF(pFuncName);
	Py_DECREF(pDataFile);
	Py_DECREF(pCallReturn); //Apparently DECREF here only once is OK, doing it in the loop is not.
	
	isLearningFlag = false;
}

/*
* The whole situation with PyObject_CallMethodObjArgs in the while loop is very confusing. 
* Most likely I am missing something here, so let me write down what works and what doesn't for future references.
*
* The method called through PyObject_CallMethodObjArgs MUST return something, otherwise Python will crash, but only
* when the program will try to shut down the interpreter (throw a tstate was nulllptr exception at program exit)
*
* The pointer returned by PyObject_CallMethodObjArgs can be safely ignored and no if no DECREF is called on it, it seems to be fine.
*
* Calling DECREF on the pointer within the loop (which should be the correct way to do it since PyObject_CallMethodObjArgs is supposed
* to allocate memory) causes a crash at shutdown (tstate exception)
*
* Calling DECREF ONCE, after the loop is fine. 
*
* As far as I can tell, tstate is a shorthand for ThreadState, so this is probably related to the multithreading side of things. 
*
* ALthough it seems unecessary, I'll store the return value of PyObject_CallMethodObjArgs in a variable as a reminder that the method
* MUST return a value, and that only one DECREF is needed. 
*
*/
