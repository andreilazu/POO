#pragma once

#include "entities/Entity.hpp"

#include <string>

class EntityInspector {
public:
    static float gatherRadiusFor(const Entity& entity);
    static int combatPowerFor(const Entity& entity);
    static std::string describeForHud(const Entity& entity);
};
