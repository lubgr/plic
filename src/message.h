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

            void append(const char *fmt, std::va_list args);
            /* Returns the number of format specifier identified by regex parsing: */
            std::ptrdiff_t variadicAppend(const char *fmt, ...);

            static void setPrintfForwarding(bool value);

            std::string getText() const;
            const Level& getLevel() const;
            const std::string& getLogger() const;

        private:
            std::ptrdiff_t getNumFmtSpecifier(const std::string& fmt) const;
            std::ptrdiff_t count(const std::string& fmt, const std::string& pattern) const;

            std::stringstream stream;
            std::string logger;
            Level level;
    };
}

#endif
