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

    void debug(const std::string& logger, const char *fmt, ...);
    void info(const std::string& logger, const char *fmt, ...);
    void warning(const std::string& logger, const char *fmt, ...);
    void error(const std::string& logger, const char *fmt, ...);
    void critical(const std::string& logger, const char *fmt, ...);

    /* If errors occur during configuration, they can't be traced back to specific exceptions.
     * Instead, a message is printed to stderr. */
    void configFile(const std::string& pyConfigFilename);
    void configStr(const std::string& pyCommands);
    void finalize();
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
void plic_finalize();

#ifdef __cplusplus
}
#endif

#endif
