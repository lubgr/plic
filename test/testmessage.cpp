
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

TEST(Message, linenumber)
{
    const int linenumber = 1234;
    plic::Message msg(plic::DEBUG, logger);

    msg.append(plic::LINE);
    msg.append(linenumber);

    CHECK_EQUAL(linenumber, msg.getLineNumber());
}

TEST(Message, filename)
{
    plic::Message msg(plic::DEBUG, logger);

    msg.append(plic::FILENAME);
    msg.append(__FILE__);

    CHECK_EQUAL(__FILE__, msg.getFilename());
}

TEST(Message, function)
{
    plic::Message msg(plic::DEBUG, logger);

    msg.append(plic::FCT);
    msg.append(__func__);

    CHECK_EQUAL(__func__, msg.getFunction());
}
