#pragma once

#include "core/GameException.hpp"

class ResourceLoadException : public GameException {
public:
    explicit ResourceLoadException(const std::string& resourceName);
};
