
#include <Python.h>
#include <csignal>
#include <cstring>
#include <iostream>
#include "pybackend.h"

namespace {
    PyObject *logging()
    {
        static PyObject *module(NULL);

        if (module == NULL) {
            module = PyImport_ImportModule("logging");

            if (module == NULL)
                PyErr_SetString(PyExc_ImportError, "Import of logging failed");
        }

        return module;
    }

    void initializePython()
    {
        Py_Initialize();

        /* Python implements a signal handler for SIGINT that can cause segmentation faults when
         * Stream objects call python functions in their destructors. Resetting the SIGINT handler
         * to its default resolves this problem: */
        PyOS_setsig(SIGINT, SIG_DFL);
    }

    void checkPythonVersion()
    {
        const std::string linkedVersionString(Py_GetVersion(), std::strlen(PY_VERSION));

        if (std::strcmp(PY_VERSION, linkedVersionString.c_str()) != 0)
            std::cerr << "Plic is built against python " << PY_VERSION << " but linked against "
                << linkedVersionString << std::endl;
    }

    void initialize()
    {
        PyObject *nullHandler;
        PyObject *rootLogger;
        PyObject *retValue;

        initializePython();
        checkPythonVersion();

        rootLogger = PyObject_CallMethod(logging(), "getLogger", NULL);
        nullHandler = PyObject_CallMethod(logging(), "NullHandler", NULL);

        retValue = PyObject_CallMethod(rootLogger, "addHandler", "(O)", nullHandler);

        Py_DECREF(rootLogger);
        Py_DECREF(nullHandler);
        Py_DECREF(retValue);
    }

    void initializeIfNecessary()
    {
        if (!Py_IsInitialized())
            initialize();
    }

    void logInitialized(const plic::Message& message)
    {
        static const int pyLevels[] = { 10, 20, 30, 40, 50 };
        const PyObject *text = Py_BuildValue("s", message.getText().c_str());
        PyObject *logger;
        PyObject *result;

        logger = PyObject_CallMethod(logging(), "getLogger", "(s)", message.getLogger().c_str());
        result = PyObject_CallMethod(logger, "log", "(iO)", pyLevels[message.getLevel()], text);

        Py_DECREF(result);
        Py_DECREF(logger);
        Py_DECREF(text);
    }
}

void plic::pyBackend::log(const Message& message)
{
    initializeIfNecessary();

    logInitialized(message);
}

int plic::pyBackend::configure(FILE *fp, const std::string& pyConfigFilename)
{
    initializeIfNecessary();

    return PyRun_SimpleFile(fp, pyConfigFilename.c_str());
}

int plic::pyBackend::configure(const std::string& pyCommands)
{
    initializeIfNecessary();

    return PyRun_SimpleString(pyCommands.c_str());
}
