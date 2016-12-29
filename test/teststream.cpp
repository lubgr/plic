
#include "plictests.h"

TEST_GROUP(Stream)
{
    const char *logger = "test";
};

TEST(Stream, emptyDefaultStream)
{
    const plic::Stream stream(plic::INFO, logger);

    CHECK(stream.getStream().str().empty());
}

TEST(Stream, emptyLoggerName)
{
    const plic::Stream stream(plic::INFO, "");

    CHECK(stream.getLoggerName().empty());
}

TEST(Stream, getLoggerNameAndLevel)
{
    const plic::Stream stream(plic::DEBUG, logger);

    CHECK_EQUAL(plic::DEBUG, stream.getLevel());
    CHECK_EQUAL(logger, stream.getLoggerName());
}

TEST(Stream, copyConstruct)
{
    plic::Stream from(plic::CRITICAL, logger);
    plic::Stream *to;

    from << criticalMsg;

    to = new plic::Stream(from);

    CHECK_EQUAL(plic::CRITICAL, to->getLevel());
    CHECK_EQUAL(logger, to->getLoggerName());
    CHECK_EQUAL(criticalMsg, to->getStream().str());

    delete to;
}

TEST(Stream, assignmentOperator)
{
    const std::string diffLogger("differentLoggerName");
    plic::Stream from(plic::WARNING, diffLogger);
    plic::Stream to(plic::DEBUG, logger);

    from << warningMsg;

    to = from;

    CHECK_EQUAL(plic::WARNING, to.getLevel());
    CHECK_EQUAL(diffLogger, to.getLoggerName());
    CHECK_EQUAL(warningMsg, to.getStream().str());
}

TEST(Stream, selfAssignment)
{
    plic::Stream stream(plic::ERROR, logger);

    stream = stream;

    CHECK_EQUAL(plic::ERROR, stream.getLevel());
    CHECK_EQUAL(logger, stream.getLoggerName());
}
