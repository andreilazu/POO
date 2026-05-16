#include "game/EntityInspector.hpp"

#include "entities/Creature.hpp"
#include "entities/Enemy.hpp"
#include "entities/LivingEntity.hpp"
#include "entities/Player.hpp"

#include <sstream>

float EntityInspector::gatherRadiusFor(const Entity& entity) {
    const Entity* base = &entity;
    if (const auto* creature = dynamic_cast<const Creature*>(base)) {
        return creature->creatureType() == CreatureType::Yellow ? 22.f : 16.f;
    }
    if (dynamic_cast<const Player*>(base) != nullptr) {
        return 24.f;
    }
    return 12.f;
}

int EntityInspector::combatPowerFor(const Entity& entity) {
    const LivingEntity* living = dynamic_cast<const LivingEntity*>(&entity);
    if (living == nullptr) {
        return 0;
    }
    if (const auto* creature = dynamic_cast<const Creature*>(living)) {
        return creature->power();
    }
    if (const auto* enemy = dynamic_cast<const Enemy*>(living)) {
        return enemy->tier();
    }
    return living->health();
}

std::string EntityInspector::describeForHud(const Entity& entity) {
    std::ostringstream out;
    out << entity.typeName();
    if (const auto* living = dynamic_cast<const LivingEntity*>(&entity)) {
        out << " HP:" << living->health();
        if (const auto* creature = dynamic_cast<const Creature*>(living)) {
            out << " pwr:" << creature->power();
        } else if (const auto* enemy = dynamic_cast<const Enemy*>(living)) {
            out << " tier:" << enemy->tier();
        }
    }
    return out.str();
}
