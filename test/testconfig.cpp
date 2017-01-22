
#include <Python.h>
#include <cstdio>
#include "plictests.h"

TEST_GROUP(Config)
{
    void teardown()
    {
        /* Remove all log files generated in ./misc/output: */
        PyRun_SimpleString("import os;import glob;\
                [os.remove(logFile) for logFile in glob.glob('misc/output/*.log')]");

        PyRun_SimpleString("import logging;logging.getLogger('test').handlers = []");
    }

    bool doesFileExist(const char *filename)
    {
        FILE *fp(fopen(filename, "r"));

        if (fp == NULL)
            return false;

        fclose(fp);

        return true;
    }
};

TEST(Config, config01SimpleString)
{
    const std::string expected("DEBUG:root:" + debugMsg + "\n" + "INFO:root:" + infoMsg + "\n");
    plic::configStr("import logging;\
        logging.getLogger().handlers = [];\
        logging.basicConfig(filename = 'misc/output/test01.log', level = logging.DEBUG)");

    /* logging.basicConfig() configures the root logger, so don't log to the 'test' logger: */
    plic::debug() << debugMsg;
    plic::info() << infoMsg;

    CHECK_EQUAL(expected, getContent("misc/output/test01.log"));
}

TEST(Config, config02SimpleFile)
{
    const std::string expected("test - WARNING - " + warningMsg + "\n");
    const char *outputFilename = "misc/output/test02.log";

    CHECK_FALSE(doesFileExist(outputFilename));

    plic::configFile("test/config02.py");

    plic::debug("test") << debugMsg;
    plic::info("test") << infoMsg;
    plic::warning("test") << warningMsg;

    CHECK(doesFileExist(outputFilename));

    CHECK_EQUAL(expected, getContent(outputFilename));
}

TEST(Config, config03Utf8ConfigFile)
{
    const std::string logger("test.test-äüöß");
    const std::string nonAsciiMsg("Critical msg. with ä, ö, ü, ß");
    const std::string expected(logger + " : " + nonAsciiMsg + "\n");

    plic::configFile("test/config03.py");

    plic::critical(logger) << nonAsciiMsg;

    CHECK_EQUAL(expected, getContent("misc/output/test03.log"));
}

TEST(Config, config04Latin1ConfigFile)
{
#if PY_MAJOR_VERSION > 2
    /* Everything is converted to a unicode representation in python3. */
    const std::string logger("test.test-äüöß");
#else
    const std::string logger("test.test-\xe4\xfc\xf6\xdf");
#endif
    const std::string nonAsciiMsg("Error msg. with ä, ö, ü, ß");
    const std::string expected(logger + " : " + nonAsciiMsg + "\n");

    plic::configFile("test/config04.py");

    plic::error(logger) << nonAsciiMsg;

    CHECK_EQUAL(expected, getContent("misc/output/test04.log"));
}

TEST(Config, config05FileWithSyntaxError)
{
    setupLogStream();

    plic::configFile("test/config05.py");

    plic::critical() << criticalMsg;

    CHECK(getLogString().empty());

    cleanUpLogStream();
}

TEST(Config, config06StringWithSyntaxError)
{
    setupLogStream();

    plic::configStr("&&& intended invalid syntax");

    plic::critical() << criticalMsg;

    CHECK(getLogString().empty());

    cleanUpLogStream();
}

TEST(Config, nonExistingFile)
{
    setupLogStream();

    plic::configFile("test/nonexisting.py");

    plic::critical() << criticalMsg;

    CHECK(getLogString().empty());

    cleanUpLogStream();
}
