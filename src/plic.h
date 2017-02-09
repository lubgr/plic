#ifndef PLIC_PLIC_H
#define PLIC_PLIC_H

#ifdef __cplusplus

#include "stream.h"
#include "message.h"
#include "version.h"

namespace plic {
    Stream debug(const std::string& logger = "");
    Stream info(const std::string& logger = "");
    Stream warning(const std::string& logger = "");
    Stream error(const std::string& logger = "");
    Stream critical(const std::string& logger = "");

    void log(const Message& msg);
    bool doForwardToVaList(const std::string fmt, ...);

    template<class S, class ...T> void logViaStream(Message& msg, const S& firstArg,
            const T&... args)
    {
        using expander = int[];

        msg.append(firstArg);

        (void) expander{ (msg.append(args), void(), 0)... };

        log(msg);
    }

    template <typename ...T> void log(Message& msg, const std::string& fmt, const T&... args)
    {
        if (doForwardToVaList(fmt, args...)) {
            msg.variadicAppend(fmt, args...);
            log(msg);
        } else
            logViaStream(msg, fmt, args...);
    }

    template <typename ...T> void log(Level level, const std::string& logger,
            const std::string& fmt, const T&... args)
    {
        log(Message(level, logger), fmt, args...);
    }

    template <typename ...T> void debug(const std::string& logger, const std::string& fmt,
            const T&... args)
    {
        Message msg(DEBUG, logger);

        log(msg, fmt, args...);
    }

    template <typename ...T> void debug(const std::string& logger, const char *fmt,
            const T&... args)
    {
        debug(logger, std::string(fmt), args...);
    }

    template<class ...T> void debug(const std::string& logger, const T&... args)
    {
        debug(logger, "", args...);
    }

    template <typename ...T> void info(const std::string& logger, const std::string& fmt,
            const T&... args)
    {
        Message msg(INFO, logger);

        log(msg, fmt, args...);
    }

    template <typename ...T> void info(const std::string& logger, const char *fmt, const T&... args)
    {
        info(logger, std::string(fmt), args...);
    }

    template<class ...T> void info(const std::string& logger, const T&... args)
    {
        info(logger, "", args...);
    }

    template <typename ...T> void warning(const std::string& logger, const std::string& fmt,
            const T&... args)
    {
        Message msg(WARNING, logger);

        log(msg, fmt, args...);
    }

    template <typename ...T> void warning(const std::string& logger, const char *fmt,
            const T&... args)
    {
        warning(logger, std::string(fmt), args...);
    }

    template<class ...T> void warning(const std::string& logger, const T&... args)
    {
        warning(logger, "", args...);
    }

    template <typename ...T> void error(const std::string& logger, const std::string& fmt,
            const T&... args)
    {
        Message msg(ERROR, logger);

        log(msg, fmt, args...);
    }

    template <typename ...T> void error(const std::string& logger, const char *fmt,
            const T&... args)
    {
        error(logger, std::string(fmt), args...);
    }

    template<class ...T> void error(const std::string& logger, const T&... args)
    {
        error(logger, "", args...);
    }

    template <typename ...T> void critical(const std::string& logger, const std::string& fmt,
            const T&... args)
    {
        Message msg(CRITICAL, logger);

        log(msg, fmt, args...);
    }

    template <typename ...T> void critical(const std::string& logger, const char *fmt,
            const T&... args)
    {
        critical(logger, std::string(fmt), args...);
    }

    template<class ...T> void critical(const std::string& logger, const T&... args)
    {
        critical(logger, "", args...);
    }

    /* If errors occur during configuration, they can't be traced back to specific exceptions.
     * Instead, a message is printed to stderr. */
    void configFile(const std::string& pyConfigFilename);
    void configStr(const std::string& pyCommands);

    void disablePrintfForwarding();
    void enablePrintfForwarding();
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
