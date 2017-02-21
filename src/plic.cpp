
#include <regex>
#include <iostream>
#include "plic.h"
#include "pybackend.h"

void plic::log(const Message& msg)
{
    pyBackend::log(msg);
}

void plic::shiftArgOrLog(std::ptrdiff_t&, const Message& msg)
{
    log(msg);
}

plic::Stream plic::debug(const std::string& logger)
{
    return Stream(DEBUG, logger);
}

plic::Stream plic::info(const std::string& logger)
{
    return Stream(INFO, logger);
}

plic::Stream plic::warning(const std::string& logger)
{
    return Stream(WARNING, logger);
}

plic::Stream plic::error(const std::string& logger)
{
    return Stream(ERROR, logger);
}

plic::Stream plic::critical(const std::string& logger)
{
    return Stream(CRITICAL, logger);
}

void plic::configFile(const std::string& pyConfigFilename)
{
    std::FILE *fp = std::fopen(pyConfigFilename.c_str(), "r");
    int returnValue = 1;

    if (fp == NULL)
        std::cerr << "Couldn't read " << pyConfigFilename << std::endl;
    else {
        returnValue = pyBackend::configure(fp, pyConfigFilename);
        fclose(fp);
    }

    if (returnValue != 0)
        std::cerr << "Configuration with " << pyConfigFilename << " failed" << std::endl;
}

void plic::configStr(const std::string& pyCommands)
{
    const int returnValue = pyBackend::configure(pyCommands);

    if (returnValue != 0)
        std::cerr << "Configuration by python string failed" << std::endl;
}

void plic::disableFormatStrings()
{
    Message::setFormatStrings(false);
}

void plic::enableFormatStrings()
{
    Message::setFormatStrings(true);
}

void plic::setSeparator(const std::string& sep)
{
    Message::setSeparator(sep);
}

void plic_debug(const char *logger, const char *fmt, ...)
{
    plic::Message msg(plic::DEBUG, logger);
    std::va_list args;
    va_start(args, fmt);

    msg.append(fmt, args);

    va_end(args);

    plic::log(msg);
}

void plic_info(const char *logger, const char *fmt, ...)
{
    plic::Message msg(plic::INFO, logger);
    std::va_list args;
    va_start(args, fmt);

    msg.append(fmt, args);

    va_end(args);

    plic::log(msg);
}

void plic_warning(const char *logger, const char *fmt, ...)
{
    plic::Message msg(plic::WARNING, logger);
    std::va_list args;
    va_start(args, fmt);

    msg.append(fmt, args);

    va_end(args);

    plic::log(msg);
}

void plic_error(const char *logger, const char *fmt, ...)
{
    plic::Message msg(plic::ERROR, logger);
    std::va_list args;
    va_start(args, fmt);

    msg.append(fmt, args);

    va_end(args);

    plic::log(msg);
}

void plic_critical(const char *logger, const char *fmt, ...)
{
    plic::Message msg(plic::CRITICAL, logger);
    std::va_list args;
    va_start(args, fmt);

    msg.append(fmt, args);

    va_end(args);

    plic::log(msg);
}

void plic_configFile(const char *pyConfigFilename)
{
    plic::configFile(pyConfigFilename);
}

void plic_configStr(const char *pyCommands)
{
    plic::configStr(pyCommands);
}
