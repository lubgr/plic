
#include "plictests.h"

TEST_GROUP(Defaults)
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

TEST(Defaults, emptyDefaultLogger)
{
    plic::critical("") << criticalMsg;

    CHECK(getLogString().empty());
}

TEST(Defaults, debugMessage)
{
    plic::debug("test") << debugMsg;

    CHECK(getLogString().empty());
}

TEST(Defaults, errorMessage)
{
    plic::error("test") << errorMsg;

    CHECK_EQUAL(errorMsg + "\n", getLogString());
}

TEST(Defaults, errorMessages)
{
    const std::string msg1("First error message");
    const std::string msg2("Second error message");

    plic::error("test") << msg1;
    plic::error("test") << msg2;

    CHECK_EQUAL(msg1 + "\n" + msg2 + "\n", getLogString());
}

TEST(Defaults, criticalMessage)
{
    plic::critical("test") << criticalMsg;

    CHECK_EQUAL(criticalMsg + "\n", getLogString());
}
