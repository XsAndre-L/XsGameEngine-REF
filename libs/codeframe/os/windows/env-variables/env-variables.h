#pragma once
#if defined(_WIN32) || defined(_WIN64)

#include "../windows-extension.h"

string getEnv(const string& varName);

void setEnv(const string& key, const string& value);
void setEnvVars(const EnvVars& env);

// set path env variable

#endif