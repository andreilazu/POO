#include "entities/LivingEntity.hpp"

#include <algorithm>

LivingEntity::LivingEntity(sf::Vector2f position, int health, float speed)
    : Entity(position), health_(health), speed_(speed) {}

void LivingEntity::update(sf::Time /*deltaTime*/) {}

int LivingEntity::health() const {
    return health_;
}

bool LivingEntity::isAlive() const {
    return health_ > 0;
}

float LivingEntity::speed() const {
    return speed_;
}

void LivingEntity::takeDamage(int amount) {
    health_ = std::max(0, health_ - amount);
}
