#pragma once

#include "entities/Creature.hpp"
#include "entities/Enemy.hpp"
#include "entities/Player.hpp"

#include <memory>

class EntityFactory {
public:
    static std::unique_ptr<Player> createPlayer(sf::Vector2f position);
    static std::unique_ptr<Creature> createCreature(CreatureType type, sf::Vector2f position);
    static std::unique_ptr<Enemy> createEnemy(sf::Vector2f position, int tier);
};
