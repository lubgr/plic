
#include <Python.h>
#include <cassert>
#include <fstream>
#include "pybackend.h"
#include "plictests.h"

const std::string debugMsg("Debug message");
const std::string infoMsg("Info message");
const std::string warningMsg("Warning message");
const std::string errorMsg("Error message");
const std::string criticalMsg("Critical message");

static PyObject *testHandler(NULL);
static PyObject *stream(NULL);

static bool needsInitialization = true;

namespace {
    void initializeTestSetup()
    {
        /* This will initialize the python interpreter and configure logging to its default: */
        plic::configStr("");

        needsInitialization = false;
    }

    void initTestLogging()
    {
        PyObject *logging(PyImport_ImportModule("logging"));
        PyObject *testLogger;
        PyObject *retValue;

        testHandler = PyObject_CallMethod(logging, "StreamHandler", "(O)", stream);
        testLogger = PyObject_CallMethod(logging, "getLogger", "(s)", "test");
        retValue = PyObject_CallMethod(testLogger, "addHandler", "(O)", testHandler);

        Py_DECREF(retValue);
        Py_DECREF(testHandler);
        Py_DECREF(testLogger);
        Py_DECREF(logging);
    }
}

void setupLogStream()
{
    if (needsInitialization)
        initializeTestSetup();

    const char *moduleName = PY_MAJOR_VERSION > 2 ? "io" : "StringIO";
    PyObject *io(PyImport_ImportModule(moduleName));

    assert(stream == NULL);

    stream = PyObject_CallMethod(io, "StringIO", NULL);

    if (testHandler == NULL)
        initTestLogging();

    PyObject_SetAttrString(testHandler, "stream", stream);

    Py_DECREF(io);
}

std::string getLogString()
{
    std::string logString;
    PyObject *retValue;
    PyObject* encoded;
    PyObject *string;
    char *msg;

    retValue = PyObject_CallMethod(testHandler, "flush", NULL);
    string = PyObject_CallMethod(stream, "getvalue", NULL);

#if PY_MAJOR_VERSION > 2
    encoded = PyUnicode_AsUTF8String(string);

    msg = PyBytes_AsString(encoded);
#else
    msg = PyString_AsString(string);
#endif

    logString = std::string(msg);

#if PY_MAJOR_VERSION > 2
    Py_DECREF(encoded);
#endif

    Py_DECREF(retValue);
    Py_DECREF(string);

    return logString;
}

void cleanUpLogStream()
{
    PyObject *logging(PyImport_ImportModule("logging"));
    PyObject *testLogger;
    PyObject *retValue;

    if (stream != NULL) {
        retValue = PyObject_CallMethod(stream, "close", NULL);
        Py_DECREF(retValue);
    }

    testLogger = PyObject_CallMethod(logging, "getLogger", "(s)", "test");
    retValue = PyObject_CallMethod(testLogger, "removeHandler", "(O)", testHandler);

    Py_DECREF(stream);
    Py_DECREF(testHandler);
    Py_DECREF(testLogger);
    Py_DECREF(logging);
    Py_DECREF(retValue);

    testHandler= NULL;
    stream = NULL;
}

std::string getContent(const std::string& filename)
{
    std::ifstream stream(filename);
    std::stringstream buffer;

    buffer << stream.rdbuf();

    return buffer.str();
}
