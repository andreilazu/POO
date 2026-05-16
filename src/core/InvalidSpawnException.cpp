#include "core/InvalidSpawnException.hpp"

InvalidSpawnException::InvalidSpawnException(const std::string& detail)
    : GameException("Invalid spawn: " + detail) {}
