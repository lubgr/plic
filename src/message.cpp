
#include <iostream>
#include <cstdio>
#include <cstring>
#include <regex>
#include <cassert>
#include "message.h"

namespace {
    bool areFormatStringsEnabled = true;
}

std::string plic::Message::separator = "";

plic::Message::Message(Level level, const std::string& logger) :
    linenumber(0),
    logger(logger),
    level(level),
    next(NONE)
{}

plic::Message::Message(const Message& other) :
    filename(other.filename),
    function(other.function),
    linenumber(other.linenumber),
    logger(other.logger),
    level(other.level),
    next(other.next)
{
    stream.str(other.stream.str());
}

const plic::Message& plic::Message::operator = (const Message& rhs)
{
    Message tmp(rhs);

    swap(tmp);

    return *this;
}

void plic::Message::swap(Message& other)
{
    std::swap(stream, other.stream);
    std::swap(filename, other.filename);
    std::swap(function, other.function);
    std::swap(linenumber, other.linenumber);
    std::swap(logger, other.logger);
    std::swap(level, other.level);
    std::swap(next, other.next);
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

    appendSeparatorAnd(buffer);

    delete[] buffer;
}

std::ptrdiff_t plic::Message::variadicAppend(const char *fmt, ...)
{
    std::ptrdiff_t nFmtSpecifier;
    std::va_list args;

    va_start(args, fmt);

    if (next == FILENAME || next == FCT) {
        setMetaInfo(fmt);
        nFmtSpecifier = 0;
    } else if (areFormatStringsEnabled || next == FMT) {
        next = NONE;
        append(fmt, args);
        nFmtSpecifier = std::strchr(fmt, '%') == NULL ? 0 : getNumFmtSpecifier(fmt);
    } else {
        appendSeparatorAnd(fmt);
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

void plic::Message::append(int number)
{
    if (next == LINE)
        setMetaInfo(number);
    else
        appendSeparatorAnd(number);
}

void plic::Message::append(const char *str)
{
    if (next == FILENAME || next == FCT)
        setMetaInfo(str);
    else
        appendSeparatorAnd(str);
}

void plic::Message::append(MetaInfo info)
{
    next = info;
}

void plic::Message::setMetaInfo(int linenumber)
{
    this->linenumber = linenumber;

    assert(next == LINE);

    next = NONE;
}

void plic::Message::setMetaInfo(const char *str)
{
    assert(next == FILENAME || next == FCT);

    if (next == FILENAME)
        filename = std::string(str);
    else if (next == FCT)
        function = std::string(str);

    next = NONE;
}

void plic::Message::setFormatStrings(bool value)
{
    areFormatStringsEnabled = value;
}

void plic::Message::setSeparator(const std::string& sep)
{
    separator = sep;
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

const std::string& plic::Message::getFilename() const
{
    return filename;
}

const std::string& plic::Message::getFunction() const
{
    return function;
}

int plic::Message::getLineNumber() const
{
    return linenumber;
}
