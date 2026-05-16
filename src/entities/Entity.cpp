#include "entities/Entity.hpp"

Entity::Entity(sf::Vector2f position) : position_(position) {}

Entity::~Entity() = default;

sf::Vector2f Entity::position() const {
    return position_;
}

void Entity::setPosition(sf::Vector2f position) {
    position_ = position;
}
