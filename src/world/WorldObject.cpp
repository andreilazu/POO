#include "world/WorldObject.hpp"

WorldObject::WorldObject(sf::Vector2f position, std::string name)
    : position_(position), name_(std::move(name)) {}

WorldObject::~WorldObject() = default;

std::string WorldObject::describe() const {
    return name_;
}

sf::Vector2f WorldObject::position() const {
    return position_;
}
