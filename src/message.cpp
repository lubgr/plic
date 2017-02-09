
#include <iostream>
#include <cstdio>
#include "message.h"

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

void plic::Message::variadicAppend(const char *fmt, ...)
{
    std::va_list args;
    va_start(args, fmt);

    append(fmt, args);

    va_end(args);
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
