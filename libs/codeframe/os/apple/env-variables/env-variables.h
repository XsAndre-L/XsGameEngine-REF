#pragma once
#if defined(__APPLE__)

#include "../apple-extension.h"

string getEnv(const string& varName);

void setEnv(const string& key, const string& value);
void setEnvVars(const EnvVars& env);

#endif