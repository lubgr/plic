
#include <cmath>
#include "plictests.h"

TEST_GROUP(VaLists)
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

TEST(VaLists, criticalMsg)
{
    const std::string expected("one: 1, two: 002, three: +03\n");

    plic::critical("test", "one: %d, two: %03d, three: %+03d", 1, 2, 3);

    CHECK_EQUAL(expected, getLogString());
}

TEST(VaLists, errorMsg)
{
    const std::string expected("error message, 3.1416, 3.142E+00\n");

    plic::critical("test", "%s, %0.4f, %0.3E", "error message", M_PI, M_PI);

    CHECK_EQUAL(expected, getLogString());
}
