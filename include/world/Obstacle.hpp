#pragma once

#include "world/WorldObject.hpp"

class Obstacle : public WorldObject {
public:
    Obstacle(sf::Vector2f position, float radius);

    std::string describe() const override;
    float radius() const;
    int health() const;
    int maxHealth() const;
    bool isDestroyed() const;
    void takeDamage(int amount);

private:
    float radius_;
    int health_;
    int maxHealth_;
};
