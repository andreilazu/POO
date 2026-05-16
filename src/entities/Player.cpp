#include "entities/Player.hpp"

#include <cmath>

void Player::tickInvulnerability(sf::Time deltaTime) {
    if (invulnTime_ > 0.f) {
        invulnTime_ -= deltaTime.asSeconds();
        if (invulnTime_ < 0.f) {
            invulnTime_ = 0.f;
        }
    }
}

bool Player::canTakeDamage() const {
    return invulnTime_ <= 0.f && isAlive();
}

void Player::onDamaged() {
    invulnTime_ = 1.75f;
}

Player::Player(sf::Vector2f spawnPosition)
    : LivingEntity(spawnPosition, 120, 120.f),
      shape_(18.f) {
    shape_.setFillColor(sf::Color(80, 200, 120));
    shape_.setOrigin({18.f, 18.f});
}

void Player::update(sf::Time deltaTime) {
    tickInvulnerability(deltaTime);
    shape_.setPosition(position_);
    if (invulnTime_ > 0.f) {
        const float blink = std::fmod(invulnTime_ * 12.f, 1.f);
        shape_.setFillColor(sf::Color(80, 200, 120, blink > 0.5f ? 120 : 255));
    } else {
        shape_.setFillColor(sf::Color(80, 200, 120));
    }
}

void Player::draw(sf::RenderTarget& target) const {
    target.draw(shape_);
}

const char* Player::typeName() const {
    return "Player";
}

float Player::radius() {
    return kRadius;
}

int Player::nectarCollected() const {
    return nectarCollected_;
}

void Player::addNectar(int amount) {
    nectarCollected_ += amount;
}

bool Player::spendNectar(int amount) {
    if (nectarCollected_ < amount) {
        return false;
    }
    nectarCollected_ -= amount;
    return true;
}

void Player::move(sf::Vector2f direction, sf::Time deltaTime) {
    const float seconds = deltaTime.asSeconds();
    position_ += direction * speed_ * seconds;
    shape_.setPosition(position_);
}
