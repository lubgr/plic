#ifndef PLIC_MESSAGE_H
#define PLIC_MESSAGE_H

#include <string>
#include <cstdarg>
#include <sstream>
#include "level.h"

namespace plic {
    class Message {
        public:
            Message(Level level, const std::string& logger);
            Message(const Message& other);
            const Message& operator = (const Message& rhs);

            template<class T> void append(const T& logObject)
            {
                stream << logObject;
            }

            void append(const std::string& fmt, std::va_list args);
            void append(const char *fmt, std::va_list args);
            void variadicAppend(const std::string& fmt, ...);

            std::string getText() const;
            const Level& getLevel() const;
            const std::string& getLogger() const;

        private:
            std::stringstream stream;
            std::string logger;
            Level level;
    };
}

#endif
