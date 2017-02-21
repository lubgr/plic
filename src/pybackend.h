#ifndef PLIC_PYBACKEND_H
#define PLIC_PYBACKEND_H

#include <string>
#include "message.h"

namespace plic {
    namespace pyBackend {
        void log(const Message& message);

        int configure(FILE *fp, const std::string& pyConfigFilename);
        int configure(const std::string& pyCommands);
    }
}

#endif
