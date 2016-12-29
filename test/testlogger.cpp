
#include "plictests.h"

TEST_GROUP(Logger)
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

TEST(Logger, nonExistingRootLogger)
{
    plic::error("newLogger") << errorMsg;

    CHECK(getLogString().empty());
}

TEST(Logger, nonExistingSubLogger)
{
    plic::error("test.subLogger") << errorMsg;

    CHECK_EQUAL(errorMsg + "\n", getLogString());
}
