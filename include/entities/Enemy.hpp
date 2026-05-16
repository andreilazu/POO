#pragma once

#include "entities/LivingEntity.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

class Enemy : public LivingEntity {
public:
    Enemy(sf::Vector2f position, int tier);
    Enemy(Enemy&&) noexcept = default;
    Enemy& operator=(Enemy&&) noexcept = default;

    void update(sf::Time deltaTime) override;
    void draw(sf::RenderTarget& target) const override;
    const char* typeName() const override;

    int tier() const;
    int attackDamage() const;
    void chase(sf::Vector2f target, sf::Time deltaTime);
    bool tryAttack(LivingEntity& target, sf::Time deltaTime);

    static constexpr float kRadius = 12.f;
    static constexpr float kAttackRange = 26.f;

private:
    int tier_;
    float attackCooldown_ = 0.f;
    mutable sf::RectangleShape shape_;
};
