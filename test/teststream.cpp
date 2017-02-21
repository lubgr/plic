
#include "plictests.h"

TEST_GROUP(Stream) {};

TEST(Stream, ignoreLineNumber)
{
    const int linenumber = 1234;
    plic::Stream stream(plic::DEBUG, "logger");

    stream << plic::LINE << linenumber;

    CHECK_EQUAL(linenumber, stream.getMessage().getLineNumber());
    CHECK(stream.getMessage().getText().empty());
}

TEST(Stream, ignoreFilename)
{
    plic::Stream stream(plic::DEBUG, "logger");

    stream << plic::FILENAME << __FILE__ << "some text";

    CHECK_EQUAL(__FILE__, stream.getMessage().getFilename());
    CHECK_EQUAL("some text", stream.getMessage().getText());
}

TEST(Stream, ignoreFunction)
{
    plic::Stream stream(plic::DEBUG, "logger");

    stream << plic::FCT << __func__;

    CHECK_EQUAL(__func__, stream.getMessage().getFunction());
    CHECK(stream.getMessage().getText().empty());
}

TEST(Stream, ignoreFmtSpecifier)
{
    plic::Stream stream(plic::DEBUG, "logger");

    stream << "%d " << -100;

    CHECK_EQUAL("%d -100", stream.getMessage().getText());
}

TEST(Stream, ignoreExplicitFmtSpecifier)
{
    plic::Stream stream(plic::DEBUG, "logger");

    plic::disableFormatStrings();

    stream << plic::FMT << "%d " << 1234;

    CHECK_EQUAL("%d 1234", stream.getMessage().getText());

    plic::enableFormatStrings();
}
