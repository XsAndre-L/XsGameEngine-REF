#pragma once

#include <future>
#include <iostream>

#include "../windows-extension.h"

string getProcessID();

bool runProcess(const string& cmd);
bool streamProcess(const string& cmd,
    const std::function<void(std::string_view)>& onChunk,
    DWORD* outExitCode = nullptr);

string formatOutputBuffer(const char* data, size_t len, bool flush = false);