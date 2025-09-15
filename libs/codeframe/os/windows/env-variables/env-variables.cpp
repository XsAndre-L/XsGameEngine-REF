#if defined(_WIN32) || defined(_WIN64)
#include "env-variables.h"

string getEnv(const string& varName)
{
    char* buffer = nullptr;
    size_t size = 0;
    if (_dupenv_s(&buffer, &size, varName.c_str()) == 0 && buffer) {
        string result { buffer };
        free(buffer);
        return result;
    }

    return ""; // Return an empty string instead of std::nullopt
}

// set env variables
void setEnv(const string& key, const string& value)
{
    if (_putenv_s(key.c_str(), value.c_str()) != 0) {
        console.error("Failed to set env var: " + key);
    }
}

void setEnvVars(const EnvVars& env)
{
    for (const auto& [key, value] : env) {
        if (_putenv_s(key.c_str(), value.c_str()) != 0) {
            console.error("Failed to set env var: " + key);
        }
    }
}

// set path env variable

#endif