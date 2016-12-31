
#include <Python.h>
#include "plictests.h"

TEST_GROUP(CInterface)
{
    void setup()
    {
        setupLogStream();
    }

    void teardown()
    {
        cleanUpLogStream();
    }
};

TEST(CInterface, debugMsg)
{
    plic_configStr("import logging; logging.getLogger('test').setLevel(logging.DEBUG)");

    plic_debug("test", debugMsg.c_str());

    CHECK_EQUAL(debugMsg + "\n", getLogString());
}

TEST(CInterface, emptyInfoMsg)
{
    plic_info("test", "this info message should't appear");

    CHECK(getLogString().empty());
}

TEST(CInterface, warningMsg)
{
    const std::string expected("warning 1 2 3\n");

    plic_warning("test", "warning %d %d %d", 1, 2, 3);

    CHECK_EQUAL(expected, getLogString());
}

TEST(CInterface, errorMsg)
{
    const char *msg("Let's be honest: these test exclusively beautify the code coverage");

    plic_error("test", msg);

    CHECK_EQUAL(std::string(msg) + "\n", getLogString());
}

TEST(CInterface, fatalMsg)
{
    plic_critical("test.sublogger", criticalMsg.c_str());

    CHECK_EQUAL(criticalMsg + "\n", getLogString());
}

TEST(CInterface, configFile)
{
    plic_configFile("test/configfromc.py");

    plic_info("c.sublogger", infoMsg.c_str());

    CHECK_EQUAL("c.sublogger : " + infoMsg + "\n", getContent("misc/output/testfromc.log"));

    PyRun_SimpleString("import os; os.remove('misc/output/testfromc.log')");
}
