
// Ugly hack to let python work without calling the debug dll...
// See https://bugs.python.org/issue22411
#ifdef _DEBUG
#undef _DEBUG
#include <Python/python.h>
#define _DEBUG
#else
#include <Python/python.h>
#endif

#include "SFML\Graphics.hpp"

#include "ProtoEng\gameplayManager.h"
#include "ProtoEng\GameWindow.h"

#include "TetrisGame.hpp"
#include "Settings.hpp"
#include "SettingsWrapper.hpp"

#include "pugiXML\pugixml.hpp"

int WinMain()
{
	bool pythonTest = false;

	if (pythonTest)
	{
		freopen("./stdout", "w", stdout);
		freopen("./stderr", "w", stderr);
		fprintf(stdout, "test out\n");

		PyObject *pName, *pMethodName, *pModule, *pFunc;
		PyObject *pyClassifier;


		Py_Initialize();

		pName = PyUnicode_DecodeFSDefault("DummyClassifier");
		// Error checking of pName left out

		pModule = PyImport_Import(pName);
		Py_DECREF(pName);

		if (pModule != NULL)
		{
			pFunc = PyObject_GetAttrString(pModule, "getDummy");
			// pFunc is a new reference

			if (pFunc && PyCallable_Check(pFunc))
			{
				pyClassifier = PyObject_CallObject(pFunc, NULL);

				if (pyClassifier != NULL)
				{
					pMethodName = PyUnicode_DecodeFSDefault("Predict");

					for (int i = 0; i < 10; ++i)
					{
						if (PyObject_CallMethodObjArgs(pyClassifier, pMethodName, NULL) == NULL) return 2;
						if (PyObject_CallMethod(pyClassifier, "Fit", NULL) == NULL) return 2;
					}

					Py_DECREF(pMethodName);
					Py_DECREF(pyClassifier);
				}
				else
				{
					Py_DECREF(pFunc);
					Py_DECREF(pModule);
					PyErr_Print();
					fprintf(stderr, "Call failed\n");
					return 1;
				}
			}
			else
			{
				if (PyErr_Occurred())
					PyErr_Print();
				fprintf(stderr, "Cannot find function \"%s\"\n", "nem");
			}
			Py_XDECREF(pFunc);
			Py_DECREF(pModule);
		}
		else
		{
			PyErr_Print();
			fprintf(stderr, "Failed to load \"%s\"\n", "nam");
			return 1;
		}

		Py_Finalize();
	}
	
	else
	{
		//Load settings
		SettingsWrapper wrapper;
		wrapper.loadSettings(SETTING_PATH);
		Settings::link(wrapper);

		TetrisGame game;

		//pugi::xml_document toto();

		game.launch();
	}

	return 0;
}