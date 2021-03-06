#ifndef PLIC_PLIC_H
#define PLIC_PLIC_H

#ifdef __cplusplus

#include <type_traits>
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
    void shiftArgOrLog(std::ptrdiff_t&, const Message& msg);

    template<class S, class... T> void log(Message& msg, const S& firstArg, const T&... args)
    {
        msg.append(firstArg);

        log(msg, args...);
    }

    template<class T> typename
        std::enable_if<std::is_trivial<T>::value, const T&>::type pass(const T& arg)
    {
        return arg;
    }

    template<class T> typename
        std::enable_if<!std::is_trivial<T>::value, std::nullptr_t>::type pass(const T&)
    {
        return nullptr;
    }

    template<class... T> void log(Message& msg, const char *fmt, const T&... args)
    {
        /* Forwarding the plain argument pack to the variadic Message method does not compile with
         * clang and is only conditionally implemented in g++, because macro-based variadic function
         * calls for non-trivial argument types are "conditionally-supported with
         * implementation-defined semantics" (C++11, §5.2.2/7). It's possible though to turn
         * non-trivial arguments into null-pointer, which can't discard information because there
         * are no format specifier for non-trivial types and such a call would be illegal anyhow. */
        std::ptrdiff_t n = msg.variadicAppend(fmt, pass(args)...);

        shiftArgOrLog(n, msg, args...);
    }

    template<class S, class... T> void shiftArgOrLog(std::ptrdiff_t& nLeft, Message& msg,
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
    void setSeparator(const std::string& sep);
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
