
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

    PyObject *createLogArgument(const plic::Message& msg)
    {
        static const int pyLevels[] = { 10, 20, 30, 40, 50 };

        return Py_BuildValue("(is)", pyLevels[msg.getLevel()], msg.getText().c_str());
    }

    PyObject *createLogKwarg(const plic::Message& msg)
    {
        const std::string& filename(msg.getFilename());
        const std::string& function(msg.getFunction());
        PyObject *functionName;
        PyObject *lineNumber;
        PyObject *fileName;
        PyObject *metaInfo;
        PyObject *extra;

        metaInfo = PyDict_New();
        extra = PyDict_New();

        lineNumber = Py_BuildValue("i", msg.getLineNumber());
        fileName = filename.empty() ? Py_None : Py_BuildValue("s", filename.c_str());
        functionName = filename.empty() ? Py_None : Py_BuildValue("s", function.c_str());

        PyDict_SetItemString(metaInfo, "cfilename", fileName);
        PyDict_SetItemString(metaInfo, "cfuncName", functionName);
        PyDict_SetItemString(metaInfo, "clineno", lineNumber);
        PyDict_SetItemString(extra, "extra", metaInfo);

        Py_DECREF(lineNumber);
        Py_DECREF(metaInfo);

        if (fileName != Py_None)
            Py_DECREF(fileName);

        if (functionName != Py_None)
            Py_DECREF(functionName);

        return extra;
    }

    void logInitialized(const plic::Message& message)
    {
        PyObject *logger;
        PyObject *result;
        PyObject *logFct;
        PyObject *extra;
        PyObject *args;

        logger = PyObject_CallMethod(logging(), "getLogger", "(s)", message.getLogger().c_str());
        logFct = PyObject_GetAttrString(logger, "log");
        args = createLogArgument(message);
        extra = createLogKwarg(message);

        result = PyObject_Call(logFct, args, extra);

        Py_DECREF(logFct);
        Py_DECREF(result);
        Py_DECREF(logger);
        Py_XDECREF(extra);
        Py_DECREF(args);
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
