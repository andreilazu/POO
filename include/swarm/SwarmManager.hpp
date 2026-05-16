#pragma once

#include "entities/Creature.hpp"
#include "entities/Entity.hpp"
#include "entities/Player.hpp"
#include "world/ProceduralGenerator.hpp"

#include <list>
#include <memory>
#include <vector>

enum class SwarmTask { Follow, Gather, Attack };

class SwarmManager {
public:
    SwarmManager();

    void addCreature(Creature creature);
    void assignTask(Entity& entity, SwarmTask task);
    void sortByDistanceTo(const Player& player);
    void update(sf::Time deltaTime, const Player& player, ProceduralGenerator& world);
    void removeDeadCreatures();
    void renderBlob(sf::RenderTarget& target, sf::Vector2f anchor) const;

    const std::vector<std::unique_ptr<Creature>>& creatures() const;
    static int totalCreaturesSpawned();

private:
    std::vector<std::unique_ptr<Creature>> creatures_;
    std::list<std::string> taskLog_;
    static int totalSpawned_;
};
