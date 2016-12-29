#ifndef PLIC_PYTHONBACKEND_H
#define PLIC_PYTHONBACKEND_H

#include <string>
#include "level.h"

namespace plic {
    namespace pyBackend {
        void log(Level level, const std::string& loggerName, const std::string& msg);

        int configure(FILE *fp, const std::string& pyConfigFilename);
        int configure(const std::string& pyCommands);
        void finalize();
    }
}

#endif
