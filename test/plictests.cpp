
#include <Python.h>
#include <cassert>
#include "pybackend.h"
#include "plictests.h"

const std::string debugMsg("Debug message");
const std::string infoMsg("Info message");
const std::string warningMsg("Warning message");
const std::string errorMsg("Error message");
const std::string criticalMsg("Critical message");

static PyObject *testHandler(NULL);
static PyObject *stream(NULL);

namespace {
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
    PyObject *retValue;
    PyObject *string;
    char *msg;

    retValue = PyObject_CallMethod(testHandler, "flush", NULL);

    Py_DECREF(retValue);

    string = PyObject_CallMethod(stream, "getvalue", NULL);

#if PY_MAJOR_VERSION > 2
    PyObject* encoded = PyUnicode_AsUTF8String(string);

    msg = PyBytes_AsString(encoded);

    Py_DECREF(encoded);
#else
    msg = PyString_AsString(string);
#endif

    Py_DECREF(retValue);
    Py_DECREF(string);

    return std::string(msg);
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
