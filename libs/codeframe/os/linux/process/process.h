#pragma once

#if defined(__linux__)
#include "../linux-extension.h"

#include <unistd.h>

string getProcessID();
bool runProcess(const char* cmd);

#endif