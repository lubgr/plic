#ifndef PLIC_STREAM_H
#define PLIC_STREAM_H

#include <string>
#include "level.h"
#include "message.h"

namespace plic {
    class Stream {
        /* Log stream object to be used like std::ostream. The operator << redirects to the
         * identical operator for std::ostream operands. Actual output is generated inside of the
         * destructor. */
        public:
            Stream(Level level, const std::string& logger);
            ~Stream();

            template <class T> Stream& operator << (const T& rhs)
            {
                message.append(rhs);

                return *this;
            }

        private:
            Message message;
    };
}

#endif
