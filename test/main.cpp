
#include <CppUTest/CommandLineTestRunner.h>
#include "plic.h"

int main(int argc, char **argv)
{
    const int retValue = CommandLineTestRunner::RunAllTests(argc, argv);

    plic::finalize();

    return retValue;
}
