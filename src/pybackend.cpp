
#include <Python.h>
#include <patchlevel.h>
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

    bool initialize()
    {
        PyObject *nullHandler;
        PyObject *rootLogger;
        PyObject *retValue;

        if (!Py_IsInitialized())
            Py_Initialize();

        rootLogger = PyObject_CallMethod(logging(), "getLogger", NULL);
        nullHandler = PyObject_CallMethod(logging(), "NullHandler", NULL);

        retValue = PyObject_CallMethod(rootLogger, "addHandler", "(O)", nullHandler);

        Py_DECREF(rootLogger);
        Py_DECREF(nullHandler);
        Py_DECREF(retValue);

        return true;
    }
}

static const bool init = initialize();

void plic::pyBackend::log(Level level, const std::string& loggerName, const std::string& msg)
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

int plic::pyBackend::configure(FILE *fp, const std::string& pyConfigFilename)
{
    return PyRun_SimpleFile(fp, pyConfigFilename.c_str());
}

int plic::pyBackend::configure(const std::string& pyCommands)
{
    return PyRun_SimpleString(pyCommands.c_str());
}

void plic::pyBackend::finalize()
{
    Py_Finalize();
}
