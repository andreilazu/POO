#include "swarm/CombatSystem.hpp"

#include "entities/Creature.hpp"
#include "entities/Enemy.hpp"
#include "entities/LivingEntity.hpp"

void CombatSystem::resolve(Entity& attacker, Entity& target) {
    const auto* attackerLiving = dynamic_cast<LivingEntity*>(&attacker);
    auto* targetLiving = dynamic_cast<LivingEntity*>(&target);
    if (attackerLiving == nullptr || targetLiving == nullptr) {
        return;
    }

    int damage = 1;
    if (const auto* creature = dynamic_cast<const Creature*>(&attacker)) {
        damage = creature->power();
        if (creature->creatureType() == CreatureType::Red) {
            damage += 1;
        }
    }
    if (const auto* enemy = dynamic_cast<const Enemy*>(&target)) {
        damage += enemy->tier();
    }

    targetLiving->takeDamage(damage);
}
