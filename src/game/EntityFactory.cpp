#include "game/EntityFactory.hpp"

#include "core/InvalidSpawnException.hpp"

std::unique_ptr<Player> EntityFactory::createPlayer(sf::Vector2f position) {
    return std::make_unique<Player>(position);
}

std::unique_ptr<Creature> EntityFactory::createCreature(CreatureType type, sf::Vector2f position) {
    if (position.x < -10000.f || position.y < -10000.f) {
        throw InvalidSpawnException("position out of bounds");
    }
    auto creature = std::make_unique<Creature>(type);
    creature->setPosition(position);
    return creature;
}

std::unique_ptr<Enemy> EntityFactory::createEnemy(sf::Vector2f position, int tier) {
    if (tier < 0) {
        throw InvalidSpawnException("negative enemy tier");
    }
    return std::make_unique<Enemy>(position, tier);
}
