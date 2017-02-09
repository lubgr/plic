
#include <regex>
#include <iostream>
#include "plic.h"
#include "pybackend.h"

namespace {
    bool isPrintfForwardingEnabled = true;

    std::ptrdiff_t getNumFmtSpecifierByParsing(const std::string& fmt)
    {
        static const std::regex doublePercent("%%");
        /* This regex is constructed following the docs for the format parameter of std::printf: */
        static const std::regex pattern(
                "%[-+ #0]*[0-9*]?(\\.[0-9*])?(h|hh|l|ll|L|z|j|t)?[csdioxXufFeEaAgGnp%]");
        const auto startPercent(std::sregex_iterator(fmt.begin(), fmt.end(), doublePercent));
        const auto start(std::sregex_iterator(fmt.begin(), fmt.end(), pattern));
        const std::ptrdiff_t nPercentFmtSpecifier = std::distance(startPercent, std::sregex_iterator());
        const std::ptrdiff_t nFmtSpecifier = std::distance(start, std::sregex_iterator());

        return nFmtSpecifier - nPercentFmtSpecifier;
    }
}

void plic::log(const Message& msg)
{
    pyBackend::log(msg);
}

void plic::shiftArgOrLog(std::ptrdiff_t, const Message& msg)
{
    log(msg);
}

std::ptrdiff_t plic::getNumFmtSpecifier(const std::string& fmt)
{
    if (isPrintfForwardingEnabled)
        return getNumFmtSpecifierByParsing(fmt);
    else
        return -1;
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
