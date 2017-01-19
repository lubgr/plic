#ifndef PLIC_STREAM_H
#define PLIC_STREAM_H

#include <string>
#include <sstream>
#include "level.h"

namespace plic {
    class Stream {
        /* Log stream object to be used like std::ostream. The operator << redirects to the
         * identical operator for std::ostream operands. Actual output is generated inside of the
         * destructor. */
        public:
            Stream(Level level, const std::string& loggerName);
            Stream(const Stream& other);
            const Stream& operator = (const Stream& rhs);
            ~Stream();

            template <class T> Stream& operator << (const T& rhs)
            {
                stream << rhs;

                return *this;
            }

            Level getLevel() const;
            const std::string& getLoggerName() const;
            const std::stringstream& getStream() const;

        private:
            Level level;
            std::string loggerName;
            std::stringstream stream;
    };
}

#endif
