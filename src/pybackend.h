#ifndef PLIC_PYBACKEND_H
#define PLIC_PYBACKEND_H

#include <string>
#include "level.h"

namespace plic {
    namespace pyBackend {
        void log(Level level, const std::string& loggerName, const std::string& msg);

        int configure(FILE *fp, const std::string& pyConfigFilename);
        int configure(const std::string& pyCommands);
    }
}

#endif
