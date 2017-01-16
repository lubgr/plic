
#include "plictests.h"

TEST_GROUP(VariadicArgs)
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

TEST(VariadicArgs, criticalMsgVariadicArgs)
{
    const std::string expected("1 2 3 4\n");

    plic::critical("test", 1, " ", 2, " ", 3, " ", 4);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, criticalMsgValist)
{
    const std::string fmt("%05d %+2d %.3f");
    const std::string expected("-0176 +2 2.346\n");

    plic::critical("test",  fmt, -176, 2, 2.3456789);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, criticalMsgValistByDecision)
{
    const std::string expected("fmt string with 2 specifier\n");

    plic::critical("test", "fmt string with %d %s", 2, "specifier");

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, criticalMsgVariadicArgsByDecision)
{
    const std::string expected("possible fmt without percent sign 123\n");

    plic::critical("test", "possible fmt without percent sign ", 1, 2, 3);

    CHECK_EQUAL(expected, getLogString());
}
