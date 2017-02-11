
#include <iostream>
#include <cstdio>
#include <regex>
#include "message.h"

namespace {
    bool isPrintfForwardingEnabled = true;
}

plic::Message::Message(Level level, const std::string& logger) :
    logger(logger),
    level(level)
{}

plic::Message::Message(const Message& other)
{
    level = other.level;
    logger = other.logger;
    stream.str(other.stream.str());
}

const plic::Message& plic::Message::operator = (const Message& rhs)
{
    if (this == &rhs)
        return *this;

    level = rhs.level;
    logger = rhs.logger;
    stream.str(rhs.stream.str());

    return *this;
}

void plic::Message::append(const char *fmt, std::va_list args)
{
    std::va_list argsCopy;
    std::size_t bufferSize;
    int charsWritten;
    char *buffer;

    va_copy(argsCopy, args);

    charsWritten = std::vsnprintf(NULL, 0, fmt, args);

    if (charsWritten < 0) {
        std::cerr << "Couldn't construct message from format specifier" << std::endl;
        bufferSize = 10000;
    } else
        bufferSize = static_cast<size_t>(charsWritten + 1);

    buffer = new char[bufferSize];

    std::vsnprintf(buffer, bufferSize, fmt, argsCopy);

    append(buffer);

    delete[] buffer;
}

std::ptrdiff_t plic::Message::variadicAppend(const char *fmt, ...)
{
    std::ptrdiff_t nFmtSpecifier;
    std::va_list args;

    va_start(args, fmt);

    if (isPrintfForwardingEnabled) {
        append(fmt, args);
        nFmtSpecifier = getNumFmtSpecifier(fmt);
    } else {
        append(fmt);
        nFmtSpecifier = 0;
    }

    va_end(args);

    return nFmtSpecifier;
}

std::ptrdiff_t plic::Message::getNumFmtSpecifier(const std::string& fmt) const
    /* The regex pattern are constructed following the docs of std::printf. */
{
    static const std::string start("%[-+ #0]*");
    static const std::string end("(h|hh|l|ll|L|z|j|t)?[csdioxXufFeEaAgGnp%]");
    const std::ptrdiff_t nFmtSpecifier = count(fmt, start + "[0-9*]*(\\.[0-9*]*)?"  + end);
    const std::ptrdiff_t nAdditionalFirstInt = count(fmt, start + "\\*(\\.[0-9*]*)?" + end);
    const std::ptrdiff_t nAdditionalSecondInt = count(fmt, start + "[0-9*]*\\.\\*" + end);
    const std::ptrdiff_t nPercentFmtSpecifier = count(fmt, "%%");

    return nFmtSpecifier - nPercentFmtSpecifier + nAdditionalFirstInt + nAdditionalSecondInt;
}

std::ptrdiff_t plic::Message::count(const std::string& fmt, const std::string& pattern) const
{
    const std::regex regex(pattern);
    const auto start(std::sregex_iterator(fmt.begin(), fmt.end(), regex));

    return std::distance(start, std::sregex_iterator());
}

void plic::Message::setPrintfForwarding(bool value)
{
    isPrintfForwardingEnabled = value;
}

std::string plic::Message::getText() const
{
    return stream.str();
}

const plic::Level& plic::Message::getLevel() const
{
    return level;
}

const std::string& plic::Message::getLogger() const
{
    return logger;
}
