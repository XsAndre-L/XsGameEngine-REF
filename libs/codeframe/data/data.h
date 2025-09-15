#pragma once

// #include "compression/compression.h"
#include "format/format.h"

#include "../core/base.h"

template <typename Source, typename T>
struct FieldMap {
    string key;
    std::function<T(const Source&)> mapper;
    T defaultType;
};

template <typename Source, typename T>
struct BiDirectionalFieldMap {
    string key;
    std::function<T(const Source&)> toValue; // Source -> T
    std::function<Source(const T&)> fromValue; // T -> Source
    T defaultType;
};
