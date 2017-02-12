
#include "plic.h"
#include <CppUTest/CommandLineTestRunner.h>

void initConstructOnFirstUse()
{
    plic::debug("", "no real %% format specifier ", 0);
}

int main(int argc, char **argv)
{
    initConstructOnFirstUse();

    return CommandLineTestRunner::RunAllTests(argc, argv);
}
