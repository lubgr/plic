
#include <Python.h>
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

    void initialize()
    {
        PyObject *nullHandler;
        PyObject *rootLogger;
        PyObject *retValue;

        Py_Initialize();

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

    void logInitialized(plic::Level level, const std::string& loggerName, const std::string& msg)
    {
        static const int pyLevels[] = { 10, 20, 30, 40, 50 };
        const PyObject *text = Py_BuildValue("s", msg.c_str());
        PyObject *logger;
        PyObject *result;

        logger = PyObject_CallMethod(logging(), "getLogger", "(s)", loggerName.c_str());
        result = PyObject_CallMethod(logger, "log", "iO", pyLevels[level], text);

        Py_DECREF(result);
        Py_DECREF(logger);
        Py_DECREF(text);
    }
}

void plic::pyBackend::log(Level level, const std::string& loggerName, const std::string& msg)
{
    initializeIfNecessary();

    logInitialized(level, loggerName, msg);
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
