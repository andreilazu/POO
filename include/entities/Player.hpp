#pragma once

#include "entities/LivingEntity.hpp"

#include <SFML/Graphics/CircleShape.hpp>

class Player : public LivingEntity {
public:
    explicit Player(sf::Vector2f spawnPosition);

    void update(sf::Time deltaTime) override;
    void draw(sf::RenderTarget& target) const override;
    const char* typeName() const override;

    void move(sf::Vector2f direction, sf::Time deltaTime);

    static constexpr float kRadius = 18.f;
    static float radius();

    int nectarCollected() const;
    void addNectar(int amount);
    bool spendNectar(int amount);

    void tickInvulnerability(sf::Time deltaTime);
    bool canTakeDamage() const;
    void onDamaged();

private:
    mutable sf::CircleShape shape_;
    int nectarCollected_ = 0;
    float invulnTime_ = 0.f;
};
