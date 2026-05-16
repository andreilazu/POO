#include "world/Obstacle.hpp"

#include <algorithm>

Obstacle::Obstacle(sf::Vector2f position, float radius)
    : WorldObject(position, "rock"),
      radius_(radius),
      health_(static_cast<int>(radius * 3.f)),
      maxHealth_(static_cast<int>(radius * 3.f)) {}

std::string Obstacle::describe() const {
    return "Obstacle(r=" + std::to_string(radius_) + ")";
}

float Obstacle::radius() const {
    return radius_;
}

int Obstacle::health() const {
    return health_;
}

int Obstacle::maxHealth() const {
    return maxHealth_;
}

bool Obstacle::isDestroyed() const {
    return health_ <= 0;
}

void Obstacle::takeDamage(int amount) {
    health_ = std::max(0, health_ - amount);
}
