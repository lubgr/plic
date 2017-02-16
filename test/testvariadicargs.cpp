
#include <cmath>
#include "plictests.h"

namespace {
    class Dummy {};

    std::ostream& operator << (std::ostream& stream, const Dummy&)
    {
        stream << "Some << operator";

        return stream;
    }
}

TEST_GROUP(VariadicArgs)
{
    Dummy dummy;

    void setup()
    {
        setupLogStream();
    }

    void teardown()
    {
        cleanUpLogStream();
    }
};

TEST(VariadicArgs, simpleDebugMsg)
{
    plic::debug("test", "%s %s", "debug", "string");

    CHECK(getLogString().empty());
}

TEST(VariadicArgs, simpleInfoMsg)
{
    plic::info("test", "%s %s", "info", "string");

    CHECK(getLogString().empty());
}

TEST(VariadicArgs, simpleWarningMsg)
{
    const std::string expected("warning string\n");

    plic::warning("test", "%s %s", "warning", "string");

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, simpleCriticalMsg)
{
    const std::string expected("one: 1, two: 002, three: +03\n");

    plic::critical("test", "one: %d, two: %03d, three: %+03d", 1, 2, 3);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, simpleErrorMsg)
{
    const std::string expected("error message, 3.1416, 3.142E+00\n");

    plic::error("test", "%s, %0.4f, %0.3E", "error message", M_PI, M_PI);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, warningMsgWithoutPrintfForwarding)
{
    const std::string expected("warning msg with format %d %s2specifier\n");

    plic::disableFormatStrings();

    plic::warning("test", "warning msg with format %d %s", 2, "specifier");

    plic::enableFormatStrings();

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, customOperator)
{
    const std::string expected("Some << operator and a second time: Some << operator\n");

    plic::error("test", dummy, " and a second time: ", dummy);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, multipleFmtStrings)
{
    const std::string expected("0 1 2 three 4 5.6 7\n");

    plic::error("test", 0, " %d %d %s", 1, 2, "three ", "%d %.1f", 4, 5.6123, " %u", 7);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, printfDoc01)
{
    const std::string expected("\t[     Hello]\n\t[Hello     ]\n\t[     Hello]\n\t[Hell      ]\n\t"
            "[Hell      ]\n\n");
    const char* s = "Hello";

    plic::error("test", "\t[%10s]\n\t[%-10s]\n\t[%*s]\n\t[%-10.*s]\n\t[%-*.*s]\n", s, s, 10, s, 4,
            s, 10, 4, s);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, printfDoc02)
{
    const std::string expected("\t1 2 000003 0  +4 4294967295\n\n");

    plic::error("test", "\t%i %d %.6i %i %.0i %+i %u\n", 1, 2, 3, 0, 0, 4, -1);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, printfDoc03)
{
    const std::string expected("\t5 a A 0x6\n\n");

    plic::error("test", "\t%x %x %X %#x\n", 5, 10, 10, 6);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, printfDoc04)
{
    const std::string expected("\t12 012 04\n\n");

    plic::error("test", "\t%o %#o %#o\n", 10, 10, 4);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, printfDoc05)
{
    const std::string expected("\t01.50 1.50  1.50\n\n");

    plic::error("test", "\t%05.2f %.2f %5.2f\n", 1.5, 1.5, 1.5);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, printfDoc06)
{
    const std::string expected("\t0x1.8p+0 0X1.8P+0\n\n");

    plic::error("test", "\t%a %A\n", 1.5, 1.5);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, printfDoc07)
{
    const std::string expected("\tA %\n'    x'\n'x    '\n\n");

    plic::error("test", "\t%c %%\n", 65, "'%*c'\n", 5, 'x', "'%*c'\n", -5, 'x');

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, emptyStrings)
{
    const std::string expected("1 2 3 4\n");

    plic::critical("test", 1, " ", 2, " ", 3, " ", 4);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, numberFmt)
{
    const std::string fmt("%05d %+2d %.3f");
    const std::string expected("-0176 +2 2.346\n");

    plic::critical("test", fmt.c_str(), -176, 2, 2.3456789);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, numberAndStringFmt)
{
    const std::string expected("fmt string with 2 specifier\n");

    plic::critical("test", "fmt string with %d %s", 2, "specifier");

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, stringWithoutFmtSpecifier)
{
    const std::string expected("possible fmt without percent sign 123\n");

    plic::critical("test", "possible fmt without percent sign ", 1, 2, 3);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, doublePercentFmt)
{
    const std::string expected("fmt % string % with % 123\n");

    plic::critical("test", "fmt %% string %% with %% ", 1, 2, 3);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VariadicArgs, multipleConversionModifier)
{
    const std::string expected("-20 \n");

    plic::critical("test", "%+---++-++4d", -20);

    CHECK_EQUAL(expected, getLogString());
}
