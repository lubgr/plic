#ifndef PLIC_PLIC_H
#define PLIC_PLIC_H

#ifdef __cplusplus

#include "stream.h"
#include "message.h"
#include "version.h"

namespace plic {
    Stream debug(const std::string& logger);
    Stream info(const std::string& logger);
    Stream warning(const std::string& logger);
    Stream error(const std::string& logger);
    Stream critical(const std::string& logger);

#if __cplusplus >= 201103L
    void log(const Message& msg);
    void shiftArgOrLog(std::ptrdiff_t, const Message& msg);

    template<class S, class... T> void log(Message& msg, const S& firstArg, const T&... args)
    {
        msg.append(firstArg);

        log(msg, args...);
    }

    template<class... T> void log(Message& msg, const char *fmt, const T&... args)
    {
        std::ptrdiff_t n = msg.variadicAppend(fmt, args...);

        shiftArgOrLog(n, msg, args...);
    }

    template<class S, class... T> void shiftArgOrLog(std::ptrdiff_t nLeft, Message& msg,
            const S& firstArg, const T&... args)
    {
        if (nLeft > 0)
            shiftArgOrLog(--nLeft, msg, args...);
        else
            plic::log(msg, firstArg, args...);
    }

    template<class... T> void debug(const std::string& logger, const char *fmt, const T&... args)
    {
        Message msg(plic::DEBUG, logger);

        log(msg, fmt, args...);
    }

    template<class S, class... T> void debug(const std::string& logger, const S& firstArg,
            const T&... args)
    {
        debug(logger, "", firstArg, args...);
    }

    template<class... T> void info(const std::string& logger, const char *fmt, const T&... args)
    {
        Message msg(plic::INFO, logger);

        log(msg, fmt, args...);
    }

    template<class S, class... T> void info(const std::string& logger, const S& firstArg,
            const T&... args)
    {
        info(logger, "", firstArg, args...);
    }

    template<class... T> void warning(const std::string& logger, const char *fmt, const T&... args)
    {
        Message msg(plic::WARNING, logger);

        log(msg, fmt, args...);
    }

    template<class S, class... T> void warning(const std::string& logger, const S& firstArg,
            const T&... args)
    {
        warning(logger, "", firstArg, args...);
    }

    template<class... T> void error(const std::string& logger, const char *fmt, const T&... args)
    {
        Message msg(plic::ERROR, logger);

        log(msg, fmt, args...);
    }

    template<class S, class... T> void error(const std::string& logger, const S& firstArg,
            const T&... args)
    {
        error(logger, "", firstArg, args...);
    }

    template<class... T> void critical(const std::string& logger, const char *fmt, const T&... args)
    {
        Message msg(plic::CRITICAL, logger);

        log(msg, fmt, args...);
    }

    template<class S, class... T> void critical(const std::string& logger, const S& firstArg,
            const T&... args)
    {
        critical(logger, "", firstArg, args...);
    }
#endif

    /* If errors occur during configuration, they can't be traced back to specific exceptions.
     * Instead, a message is printed to stderr. */
    void configFile(const std::string& pyConfigFilename);
    void configStr(const std::string& pyCommands);

    void disableFormatStrings();
    void enableFormatStrings();
}

extern "C" {
#endif

void plic_debug(const char *logger, const char *fmt, ...);
void plic_info(const char *logger, const char *fmt, ...);
void plic_warning(const char *logger, const char *fmt, ...);
void plic_error(const char *logger, const char *fmt, ...);
void plic_critical(const char *logger, const char *fmt, ...);

void plic_configFile(const char *pyConfigFilename);
void plic_configStr(const char *pyCommands);

#ifdef __cplusplus
}
#endif

#endif
