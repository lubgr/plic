
#include <Python.h>
#include <cstdio>
#include <fstream>
#include "plictests.h"

TEST_GROUP(Config)
{
    void teardown()
    {
        /* Remove all log files generated in ./misc/output: */
        PyRun_SimpleString("import os;import glob;\
                [os.remove(logFile) for logFile in glob.glob('misc/output' + '/*.log')]");

        PyRun_SimpleString("import logging;logging.getLogger('test').handlers = []");
        PyRun_SimpleString("import logging;logging.getLogger('test').setLevel(logging.DEBUG)");
    }

    std::string getContent(const std::string& filename)
    {
        std::ifstream stream(filename);
        std::stringstream buffer;

        buffer << stream.rdbuf();

        return buffer.str();
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

TEST(Config, config01SimpelString)
{
    const std::string expected("DEBUG:test:" + debugMsg + "\n" + "INFO:test:" + infoMsg + "\n");
    plic::configStr("import logging;\
        logging.getLogger().handlers = [];\
        logging.basicConfig(filename = 'misc/output/test01.log', level = logging.DEBUG)");

    plic::debug("test") << debugMsg;
    plic::info("test") << infoMsg;

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
    const std::string logger("test.test-äüöß");
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