#pragma once
#if defined(__APPLE__)

#include "../apple-extension.h"

string getProcessID();
bool runProcess(const char* cmd);

#endif