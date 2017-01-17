
#include <CppUTest/CommandLineTestRunner.h>
#include "plic.h"

void initConstructOnFirstUse()
{
    plic::debug("", "no format specifier ", 0);
}

int main(int argc, char **argv)
{
    initConstructOnFirstUse();

    return CommandLineTestRunner::RunAllTests(argc, argv);
}
