#pragma once

#include "core/GameException.hpp"

class InvalidSpawnException : public GameException {
public:
    explicit InvalidSpawnException(const std::string& detail);
};
