#include "env-variables.h"

#if defined(__APPLE__)

string getEnv(const string& varName)
{
    if (const char* envVar = std::getenv(varName.c_str())) {
        return string { envVar };
    } else
        return ""; // Return an empty string instead of std::nullopt
}

// set env variables
void setEnv(const string& key, const string& value)
{
    if (setenv(key.c_str(), value.c_str(), 1) != 0) {
        console.error("Failed to set env var: " + key);
    }
}

void setEnvVars(const EnvVars& env)
{
    for (const auto& [key, value] : env) {
        if (setenv(key.c_str(), value.c_str(), 1) != 0) {
            console.error("Failed to set env var: " + key);
        }
    }
}

#endif