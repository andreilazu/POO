#pragma once

#include "entities/Entity.hpp"

class CombatSystem {
public:
    static void resolve(Entity& attacker, Entity& target);
};
