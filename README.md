
# plic: Python Logging in C/C++ [![Build Status](https://travis-ci.org/lubgr/plic.svg?branch=develop)](https://travis-ci.org/lubgr/plic)

This library provides a convenient way to use the [python logging
module](https://docs.python.org/3/library/logging.html) from C or C++ applications. Logging can be
configured with a short python script or string, and a fancy mix of printf-format strings with
variadic template parameters or streams can be used from client code. As all logging functionality
is forwarded to the CPython implementation, the core of this library is small. The simplistic
macro-free API with, the power of the python logging module and python itself probably being already
installed on your system may be a reason to use it.

Installation
------------
To compile plic, you need [python](https://python.org) together with development files  and
[scons](http://scons.org) as a build system (thus, on Debian-based systems: `apt-get install
python-dev scons`). For unit tests, [CppUTest](http://cpputest.github.io) is required. plic should
build on all major Linux distributions by
```bash
scons lib
```
Standard environment variables `CXX`, `CXXFLAGS`, `LIBS`, `LDFLAGS` and `CPPFLAGS` are taken into
account, but can be specified as arguments to the scons invocation, too (e.g. `scons CXX=clang++`).
clang and gcc have been tested, their version must be recent enough to support C++11. If no build
options are given, the python version is detected by the default `python` executable in your $PATH
(python 2.7, 3.4m, 3.5m and 3.6m have been tested). To install header files, the shared library and
some example configuration:
```bash
scons install PREFIX=/usr/local
```
For all available build options and targets, see `scons -h` for details.

Usage from C++
--------------
First, setup the logging in a python script:
```python
import logging

formatter = logging.Formatter('%(name)s - %(levelname)s - %(message)s')

fileHandler = logging.FileHandler('messages.log', 'w')
fileHandler.setFormatter(formatter)

logger = logging.getLogger('example-logger')
logger.addHandler(fileHandler)
logger.setLevel(logging.WARNING)
```
Now, the C++ client code can refer to the config file and use variadic template calls with printf
format strings or or streams.
```c++
#include "plic/plic.h"

int main(int argc, char **argv)
{
    plic::configFile("logsetup.py"); /* The file from above. */

    plic::error("example-logger", "Option %d is to use %s", 1, "printf-like format strings");

    plic::error("example-logger", "... wherever it pleases", " %c%c%c", 'y', 'o', 'u');

    plic::error("example-logger", "Option ", 2, ": pass anything with a << ", 'o', 'p', 'e', "rator");

    plic::error("example-logger") << "Option " << 3 << " is using streams";

    return 0;
}
```
The identifier 'example-logger' in the config script and the client code must of course match.
Available function names for logging are debug/info/warning/error/fatal according to the python log
levels. When you don't feel comfortable with the type-unsafe format specifier invocation, you can
disable it. Another option is to add a separator string to be inserted between logging arguments
(the default separator is an empty string),
```c++
plic::disableFormatStrings(); /* re-enable it with plic::enableFormatStrings() */

plic::setSeparator(", ");
```
You can pass meta information like filename, line number and function name for later retrieval by an
appropriate logging formatter, see below.

Usage from C
------------
The interface for C code differs very little. Streams, variadic templates and the library namespace
obviously can't be used, instead, there are global functions prefixed with plic\_
```c
plic_configFile("logsetup.py");

plic_warning("example-logger", "A %s-character will automatically be added here", "newline");
```

Compiling the example above
---------------------------
The exemplary C++ program can be compiled with
```bash
g++ -o example snippet-from-above.cpp -lplic -lpython
```
Only a few functions from the python API are used, and they seem to be compatible between different
versions. You should nevertheless link your application against the same python library that has
been used for compiling plic.

Meta information
----------------
It is possible to pass meta information for every log record by three preceding flags,
```c++
plic::error("example-logger", plic::FILENAME, __FILE__, plic::LINE, __LINE__, plic::FCT, __func__);
```
They aren't printed by default, but passed to every logging call of the python backend such that
they can be retrieved by adjusting the formatter,
```python
formatter = logging.Formatter('%(cfilename)s: %(clineno)d [%(cfuncName)s] - %(message)s')
```
You probably don't want to type that for every logging call, so define your own [variadic
macros](https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html), e.g.
```c++
#define INFO(...) plic::info("myapp", plic::FILENAME, __FILE__, plic::LINE, __LINE__, __VA_ARGS__)
```
that can then be invoked by
```c++
INFO("We can output where this messages was logged");
```
As use and implementation of macros like this are a matter of taste, such shortcuts aren't provided
by plic, you'll have to define them by your own.

Additional notes
-------------
* By default, logging is disabled, you have to explicitly enable it (see [here]
  (https://docs.python.org/3/howto/logging.html#configuring-logging-for-a-library) for a reasoning).
* Two example configuration files for printing messages to stdout and for logging to a file are
  provided in `misc/examples` or  after installing in `[PREFIX]/share/plic`.
* When configuring logging with the
  [logging.basicConfig](https://docs.python.org/3/library/logging.html#logging.basicConfig)
  function, the default (Null-)Handler must be removed from the root logger before by
  `logging.getLogger().handlers = []` in the config script.
* When using streams from C++, a wrapper class is involved that will invoke the python backend
  during its destruction. Thus, you probably don't want static Stream objects, and the easiest way
  of using streams is to rely on temporary objects as shown above.
* With older compilers, features that came with C++11 or above may not be available. In that case,
  logging via streams is the only option (variadic template functions aren't defined).
* Format specifier work only with string literals, not with std::string or std::wchar_t or any other
  type. If a '%' is found in the string literal, a regex is used to parse the string for [valid
  format specifier](http://en.cppreference.com/w/cpp/io/c/fprintf). If none is found, all arguments
  are transformed into text by std::ostream << operators as when logging to a stream.
* If you don't want every string literal to be parsed for format specifier, disable it by
  ```plic::diasableFormatStrings()``` and then add the plic::FMT token before every format string
  that you actively want to be parsed.
* When [valgrind](http://valgrind.org) shall be used for to debug your application, it is best to
  link against a python library built with
  [--with-valgrind](http://svn.python.org/projects/python/trunk/Misc/README.valgrind) to not clutter
  the output with false positives.
