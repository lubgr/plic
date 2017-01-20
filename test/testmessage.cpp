
#include "plictests.h"

TEST_GROUP(Message)
{
    const char *logger = "test";
};

TEST(Message, emptyDefaultMsg)
{
    const plic::Message msg(plic::INFO, logger);

    CHECK(msg.getText().empty());
}

TEST(Message, emptyLoggerName)
{
    const plic::Message msg(plic::INFO, "");

    CHECK(msg.getText().empty());
}

TEST(Message, getLoggerNameAndLevel)
{
    const plic::Message msg(plic::DEBUG, logger);

    CHECK_EQUAL(plic::DEBUG, msg.getLevel());
    CHECK_EQUAL(logger, msg.getLogger());
}

TEST(Message, copyConstruct)
{
    plic::Message from(plic::CRITICAL, logger);
    plic::Message *to;

    from.append(criticalMsg);

    to = new plic::Message(from);

    CHECK_EQUAL(plic::CRITICAL, to->getLevel());
    CHECK_EQUAL(logger, to->getLogger());
    CHECK_EQUAL(criticalMsg, to->getText());

    delete to;
}

TEST(Message, assignmentOperator)
{
    const std::string diffLogger("differentLoggerName");
    plic::Message from(plic::WARNING, diffLogger);
    plic::Message to(plic::DEBUG, logger);

    from.append(warningMsg);

    to = from;

    CHECK_EQUAL(plic::WARNING, to.getLevel());
    CHECK_EQUAL(diffLogger, to.getLogger());
    CHECK_EQUAL(warningMsg, to.getText());
}

TEST(Message, selfAssignment)
{
    plic::Message msg(plic::ERROR, logger);

    msg = msg;

    CHECK_EQUAL(plic::ERROR, msg.getLevel());
    CHECK_EQUAL(logger, msg.getLogger());
}
