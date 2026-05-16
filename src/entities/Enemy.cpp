#include "entities/Enemy.hpp"

#include <cmath>

Enemy::Enemy(sf::Vector2f position, int tier)
    : LivingEntity(position, 20 + tier * 10, 40.f + tier * 5.f),
      tier_(tier),
      shape_({24.f, 24.f}) {
    shape_.setFillColor(sf::Color(180, 60, 60));
    shape_.setOrigin({12.f, 12.f});
}

void Enemy::update(sf::Time /*deltaTime*/) {
    shape_.setPosition(position_);
}

void Enemy::draw(sf::RenderTarget& target) const {
    target.draw(shape_);
}

const char* Enemy::typeName() const {
    return "Enemy";
}

int Enemy::tier() const {
    return tier_;
}

int Enemy::attackDamage() const {
    return 2 + tier_ / 2;
}

void Enemy::chase(sf::Vector2f target, sf::Time deltaTime) {
    if (!isAlive()) {
        return;
    }
    const float dt = deltaTime.asSeconds();
    if (attackCooldown_ > 0.f) {
        attackCooldown_ -= dt;
    }

    sf::Vector2f delta = target - position_;
    const float distSq = delta.x * delta.x + delta.y * delta.y;
    if (distSq < 1.f) {
        return;
    }
    const float dist = std::sqrt(distSq);
    const sf::Vector2f direction = {delta.x / dist, delta.y / dist};
    position_ += direction * speed_ * deltaTime.asSeconds();
}

bool Enemy::tryAttack(LivingEntity& target, sf::Time deltaTime) {
    if (!isAlive() || !target.isAlive() || attackCooldown_ > 0.f) {
        return false;
    }

    const sf::Vector2f delta = target.position() - position_;
    const float attackRange = kAttackRange + kRadius;
    const float rangeSq = attackRange * attackRange;
    if ((delta.x * delta.x + delta.y * delta.y) > rangeSq) {
        return false;
    }

    target.takeDamage(attackDamage());
    attackCooldown_ = 1.35f - static_cast<float>(tier_) * 0.03f;
    if (attackCooldown_ < 0.75f) {
        attackCooldown_ = 0.75f;
    }
    (void)deltaTime;
    return true;
}
