#pragma once

#include "world/WorldObject.hpp"

class Collectible : public WorldObject {
public:
    Collectible(sf::Vector2f position, int value);

    std::string describe() const override;
    int value() const;

private:
    int value_;
};
