#pragma once
#if defined(__linux__)

#include "../linux-extension.h"

string getEnv(const string& varName);

void setEnv(const string& key, const string& value);
void setEnvVars(const EnvVars& env);

#endif