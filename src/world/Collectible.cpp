#include "world/Collectible.hpp"

Collectible::Collectible(sf::Vector2f position, int value)
    : WorldObject(position, "nectar"), value_(value) {}

std::string Collectible::describe() const {
    return "Collectible(" + std::to_string(value_) + ")";
}

int Collectible::value() const {
    return value_;
}
