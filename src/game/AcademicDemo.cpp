#include "game/AcademicDemo.hpp"

#include "core/GameException.hpp"
#include "core/InvalidSpawnException.hpp"
#include "core/ResourceLoadException.hpp"
#include "entities/Creature.hpp"
#include "entities/Enemy.hpp"
#include "game/EntityFactory.hpp"
#include "game/EventBus.hpp"
#include "io/CreatureIO.hpp"
#include "swarm/CombatSystem.hpp"
#include "swarm/ObjectPool.hpp"
#include "swarm/SwarmManager.hpp"
#include "world/Collectible.hpp"
#include "world/ProceduralGenerator.hpp"
#include "world/WorldChunk.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

void AcademicDemo::run() {
    std::cout << "\n--- Demo cerinte academice ---\n";

    Creature a(CreatureType::Red);
    Creature b(CreatureType::Blue, sf::Color::Cyan, 2);
    Creature merged = a + b;
    ++merged;
    std::cout << "Creature IO: " << merged << '\n';

    std::istringstream input("yellow,3,10,20");
    Creature loaded;
    input >> loaded;
    std::cout << "Creature citit: " << loaded << '\n';

    ObjectPool<Creature> creaturePool(16);
    ObjectPool<Collectible> collectiblePool(8);
    Creature* pooled = creaturePool.acquire(CreatureType::Yellow);
    Collectible* nectar = collectiblePool.acquire(sf::Vector2f{5.f, 5.f}, 3);
    std::cout << "ObjectPool active creatures: " << creaturePool.activeCount()
              << ", collectibles: " << collectiblePool.activeCount() << '\n';
    creaturePool.release(pooled);
    collectiblePool.release(nectar);

    SwarmManager swarm;
    auto player = EntityFactory::createPlayer({0.f, 0.f});
    auto creature = EntityFactory::createCreature(CreatureType::Red, {40.f, 0.f});
    swarm.addCreature(*creature);
    Entity& asEntity = *creature;
    swarm.assignTask(asEntity, SwarmTask::Gather);
    swarm.sortByDistanceTo(*player);

    auto enemy = EntityFactory::createEnemy({80.f, 0.f}, 2);
    CombatSystem::resolve(*creature, *enemy);
    std::cout << "Enemy HP dupa combat: " << enemy->health() << '\n';

    ProceduralGenerator world(ProceduralGenerator::seed());
    WorldChunk& chunk = world.chunkAt(2, 3);
    chunk.registerOccupant(*player);
    chunk.registerOccupant(*creature);
    chunk.tickOccupants();

    EventBus bus;
    bus.subscribe("creature.collected", [](const std::string& payload) {
        std::cout << "Observer: collected " << payload << '\n';
    });
    bus.publish("creature.collected", "nectar");

    std::vector<int> values{5, 1, 4, 2};
    std::for_each(values.begin(), values.end(), [](int& v) { v *= 2; });
    std::sort(values.begin(), values.end(), [](int lhs, int rhs) { return lhs > rhs; });
    std::cout << "STL + lambda: ";
    for (int v : values) {
        std::cout << v << ' ';
    }
    std::cout << '\n';

    std::cout << "Creaturi tot spawn-ate (static): " << SwarmManager::totalCreaturesSpawned()
              << '\n';

    try {
        EntityFactory::createEnemy({0.f, 0.f}, -1);
    } catch (const InvalidSpawnException& ex) {
        std::cout << "Prins InvalidSpawnException: " << ex.what() << '\n';
    }

    try {
        throw ResourceLoadException("textures/atlas.png");
    } catch (const GameException& ex) {
        std::cout << "Prins GameException (upcast): " << ex.what() << '\n';
    } catch (const std::exception& ex) {
        std::cout << "Prins std::exception: " << ex.what() << '\n';
    }

    std::cout << "--- Demo terminat ---\n";
}
