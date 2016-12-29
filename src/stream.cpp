
#include "pybackend.h"
#include "stream.h"

plic::Stream::Stream(Level level, const std::string& loggerName) :
    level(level),
    loggerName(loggerName)
{}

plic::Stream::Stream(const Stream& other)
{
    level = other.level;
    loggerName = other.loggerName;
    stream.str(other.stream.str());
}

const plic::Stream& plic::Stream::operator = (const Stream& rhs)
{
    if (this == &rhs)
        return *this;

    level = rhs.level;
    loggerName = rhs.loggerName;
    stream.str(rhs.stream.str());

    return *this;
}

plic::Stream::~Stream()
{
    pyBackend::log(level, loggerName, stream.str());
}

plic::Level plic::Stream::getLevel() const
{
    return level;
}

const std::string& plic::Stream::getLoggerName() const
{
    return loggerName;
}

const std::stringstream& plic::Stream::getStream() const
{
    return stream;
}
