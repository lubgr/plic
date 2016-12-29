#ifndef PLIC_PLICTESTS_H
#define PLIC_PLICTESTS_H

/* Main header to be included in each compilation unit with test cases. This file includes the main
 * header of CppUTest and provides common function definitions. */

#include <iostream>
#include "CppUTest/TestHarness.h"
#include "plic.h"

void setupLogStream();
std::string getLogString();
void cleanUpLogStream();

extern const std::string debugMsg;
extern const std::string infoMsg;
extern const std::string warningMsg;
extern const std::string errorMsg;
extern const std::string criticalMsg;

#endif
