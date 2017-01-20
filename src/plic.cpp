
#include <cstdarg>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <regex>
#include "plic.h"
#include "pybackend.h"

namespace {
    bool isPrintfForwardingEnabled = true;

    bool isFormatStringByParsing(const std::string& fmt)
    {
        /* This regex is constructed following the docs for the format parameter of std::printf: */
        static const std::regex pattern(
                "%[-+ #0]?[0-9*]?(\\.[0-9*])?(h|hh|l|ll|L|z|j|t)?[csdioxXufFeEaAgGnp%]");
        std::smatch match;

        return std::regex_search(fmt, match, pattern);
    }

    bool isFormatString(const std::string& fmt, std::va_list args)
    {
        int charsWritten;

        charsWritten = std::vsnprintf(NULL, 0, fmt.c_str(), args);

        if (charsWritten >= 0 && fmt.length() != static_cast<std::size_t>(charsWritten)) {
            assert(isFormatStringByParsing(fmt));
            return true;
        }

        return isFormatStringByParsing(fmt);
    }
}

void plic::log(const Message& msg)
{
    pyBackend::log(msg);
}

bool plic::doForwardToVaList(const std::string fmt, ...)
{
    std::va_list argptr;
    bool doForward;

    va_start(argptr, fmt);

    doForward = isPrintfForwardingEnabled && isFormatString(fmt, argptr);

    va_end(argptr);

    return doForward;
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
    std::FILE *fp = fopen(pyConfigFilename.c_str(), "r");
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

void plic::disablePrintfForwarding()
{
    isPrintfForwardingEnabled = false;
}

void plic::enablePrintfForwarding()
{
    isPrintfForwardingEnabled = true;
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
