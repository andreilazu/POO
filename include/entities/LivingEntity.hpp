#pragma once

#include "entities/Entity.hpp"

class LivingEntity : public Entity {
public:
    LivingEntity(sf::Vector2f position, int health, float speed);
    LivingEntity(LivingEntity&&) noexcept = default;
    LivingEntity& operator=(LivingEntity&&) noexcept = default;

    void update(sf::Time deltaTime) override;
    int health() const;
    bool isAlive() const;
    float speed() const;
    void takeDamage(int amount);

protected:
    int health_;
    float speed_;
};
