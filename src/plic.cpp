
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include "plic.h"
#include "pybackend.h"

/* Save some typing of the va_start/va_end in functions with C-style variable arguments: */
#define logWithValist(level) std::va_list argptr; va_start(argptr, fmt); \
    logToPyBackend(level, logger, fmt, argptr); va_end(argptr);

namespace {
    std::string format(const char *fmt, std::va_list args)
    {
        std::va_list argsCopy;
        std::string result;
        size_t bufferSize;
        int charsWritten;
        char *buffer;

        va_copy(argsCopy, args);

        charsWritten = std::vsnprintf(NULL, 0, fmt, args) + 1;

        if (charsWritten < 0) {
            std::cerr << "Couldn't construct message from format specifier" << std::endl;
            bufferSize = 10000;
        } else
            bufferSize = static_cast<size_t>(charsWritten + 1);

        buffer = new char[bufferSize];

        std::vsnprintf(buffer, bufferSize, fmt, argsCopy);

        result = std::string(buffer);

        delete[] buffer;

        return result;
    }

    std::string format(const std::string& fmt, std::va_list args)
    {
        return format(fmt.c_str(), args);
    }

    void logToPyBackend(plic::Level level, const std::string& logger, const std::string& fmt,
            std::va_list args)
    {
        plic::pyBackend::log(level, logger, format(fmt, args));
    }
}

void plic::logViaVaLists(plic::Level level, const std::string& logger, const std::string fmt, ...)
{
    std::va_list argptr;
    va_start(argptr, fmt);

    logToPyBackend(level, logger, fmt, argptr);

    va_end(argptr);
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
    FILE *fp = fopen(pyConfigFilename.c_str(), "r");
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

void plic_debug(const char *logger, const char *fmt, ...)
{
    logWithValist(plic::DEBUG);
}

void plic_info(const char *logger, const char *fmt, ...)
{
    logWithValist(plic::INFO);
}

void plic_warning(const char *logger, const char *fmt, ...)
{
    logWithValist(plic::WARNING);
}

void plic_error(const char *logger, const char *fmt, ...)
{
    logWithValist(plic::ERROR);
}

void plic_critical(const char *logger, const char *fmt, ...)
{
    logWithValist(plic::CRITICAL);
}

void plic_configFile(const char *pyConfigFilename)
{
    plic::configFile(pyConfigFilename);
}

void plic_configStr(const char *pyCommands)
{
    plic::configStr(pyCommands);
}
