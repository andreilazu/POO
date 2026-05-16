#include "core/ResourceLoadException.hpp"

ResourceLoadException::ResourceLoadException(const std::string& resourceName)
    : GameException("Failed to load resource: " + resourceName) {}
