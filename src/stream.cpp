
#include "pybackend.h"
#include "stream.h"

plic::Stream::Stream(Level level, const std::string& logger) :
    message(level, logger)
{}

plic::Stream::~Stream()
{
    pyBackend::log(message);
}

const plic::Message& plic::Stream::getMessage() const
{
    return message;
}
