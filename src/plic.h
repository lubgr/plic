#ifndef PLIC_PLIC_H
#define PLIC_PLIC_H

#ifdef __cplusplus

#include "stream.h"
#include "version.h"

namespace plic {
    Stream debug(const std::string& logger = "");
    Stream info(const std::string& logger = "");
    Stream warning(const std::string& logger = "");
    Stream error(const std::string& logger = "");
    Stream critical(const std::string& logger = "");

    void logViaVaLists(Level level, const std::string& logger, const std::string fmt, ...);
    bool doForwardToVaList(const std::string fmt, ...);

    template<class S, class ...T> void logViaStream(Level level, const std::string& logger,
            S&& firstArg, T&&... args)
    {
        using expander = int[];
        Stream stream(level, logger);

        stream << std::forward<S>(firstArg);

        (void) expander{ (stream << std::forward<T>(args), void(), 0)... };
    }

    template <typename ...T> void log(Level level, const std::string& logger,
            const std::string& fmt, T&&... args)
    {
        if (doForwardToVaList(fmt, std::forward<T>(args)...))
            logViaVaLists(level, logger, fmt, std::forward<T>(args)...);
        else
            logViaStream(level, logger, fmt, std::forward<T>(args)...);
    }

    template <typename ...T> void debug(const std::string& logger, const std::string& fmt,
            T&&... args)
    {
        log(DEBUG, logger, fmt, std::forward<T>(args)...);
    }

    template <typename ...T> void debug(const std::string& logger, const char *fmt, T&&... args)
    {
        debug(logger, std::string(fmt), std::forward<T>(args)...);
    }

    template<class ...T> void debug(const std::string& logger, T&&... args)
    {
        logViaStream(DEBUG, logger, std::forward<T>(args)...);
    }

    template <typename ...T> void info(const std::string& logger, const std::string& fmt,
            T&&... args)
    {
        log(INFO, logger, fmt, std::forward<T>(args)...);
    }

    template <typename ...T> void info(const std::string& logger, const char *fmt, T&&... args)
    {
        info(logger, std::string(fmt), std::forward<T>(args)...);
    }

    template<class ...T> void info(const std::string& logger, T&&... args)
    {
        logViaStream(INFO, logger, std::forward<T>(args)...);
    }

    template <typename ...T> void warning(const std::string& logger, const std::string& fmt,
            T&&... args)
    {
        log(WARNING, logger, fmt, std::forward<T>(args)...);
    }

    template <typename ...T> void warning(const std::string& logger, const char *fmt, T&&... args)
    {
        warning(logger, std::string(fmt), std::forward<T>(args)...);
    }

    template<class ...T> void warning(const std::string& logger, T&&... args)
    {
        logViaStream(WARNING, logger, std::forward<T>(args)...);
    }

    template <typename ...T> void error(const std::string& logger, const std::string& fmt,
            T&&... args)
    {
        log(ERROR, logger, fmt, std::forward<T>(args)...);
    }

    template <typename ...T> void error(const std::string& logger, const char *fmt, T&&... args)
    {
        error(logger, std::string(fmt), std::forward<T>(args)...);
    }

    template<class ...T> void error(const std::string& logger, T&&... args)
    {
        logViaStream(ERROR, logger, std::forward<T>(args)...);
    }

    template <typename ...T> void critical(const std::string& logger, const std::string& fmt,
            T&&... args)
    {
        log(CRITICAL, logger, fmt, std::forward<T>(args)...);
    }

    template <typename ...T> void critical(const std::string& logger, const char *fmt, T&&... args)
    {
        critical(logger, std::string(fmt), std::forward<T>(args)...);
    }

    template<class ...T> void critical(const std::string& logger, T&&... args)
    {
        logViaStream(CRITICAL, logger, std::forward<T>(args)...);
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
