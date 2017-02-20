#ifndef PLIC_MESSAGE_H
#define PLIC_MESSAGE_H

#include <string>
#include <cstdarg>
#include <sstream>
#include "level.h"
#include "metainfo.h"

namespace plic {
    class Message {
        public:
            Message(Level level, const std::string& logger);
            Message(const Message& other);
            const Message& operator = (const Message& rhs);
            void swap(Message& other);

            template<class T> void append(const T& logObject)
            {
                appendSeparatorAnd(logObject);
            }

            void append(const char *fmt, std::va_list args);
            /* Returns the number of format specifier identified by regex parsing: */
            std::ptrdiff_t variadicAppend(const char *fmt, ...);
            void append(const char *str);
            void append(int number);
            void append(MetaInfo info);
            void setMetaInfo(int linenumber);
            void setMetaInfo(const char *str);

            static void setFormatStrings(bool value);
            static void setSeparator(const std::string& sep);

            std::string getText() const;
            const Level& getLevel() const;
            const std::string& getLogger() const;
            const std::string& getFilename() const;
            const std::string& getFunction() const;
            int getLineNumber() const;

        private:
            template<class T> void appendSeparatorAnd(const T& logObject)
            {
                if (stream.str().length() > 0)
                    stream << separator;

                stream << logObject;
            }

            std::ptrdiff_t getNumFmtSpecifier(const std::string& fmt) const;
            std::ptrdiff_t count(const std::string& fmt, const std::string& pattern) const;

            static std::string separator;
            std::stringstream stream;
            std::string filename;
            std::string function;
            int linenumber;
            std::string logger;
            Level level;
            MetaInfo next;
    };
}

#endif
