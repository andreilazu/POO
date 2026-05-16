#pragma once

#include "entities/Entity.hpp"
#include "entities/Enemy.hpp"
#include "world/Biome.hpp"
#include "world/Collectible.hpp"
#include "world/Obstacle.hpp"

#include <cstdint>
#include <list>
#include <memory>
#include <vector>

struct ChunkId {
    int x = 0;
    int y = 0;

    bool operator==(const ChunkId& other) const;
};

struct ChunkIdHash {
    std::size_t operator()(const ChunkId& id) const noexcept;
};

class WorldChunk {
public:
    explicit WorldChunk(ChunkId id, Biome biome);

    void addCollectible(Collectible item);
    void addObstacle(Obstacle item);
    void addEnemy(Enemy enemy);
    void registerOccupant(const Entity& entity);
    void tickOccupants();

    const ChunkId& id() const;
    Biome biome() const;
    int collectAt(sf::Vector2f center, float radius);
    int damageObstaclesAt(sf::Vector2f center, float radius, int damage);
    const std::vector<Collectible>& collectibles() const;
    const std::vector<Obstacle>& obstacles() const;
    std::vector<Enemy>& enemies();
    const std::vector<Enemy>& enemies() const;

private:
    ChunkId id_;
    Biome biome_;
    std::vector<Collectible> collectibles_;
    std::vector<Obstacle> obstacles_;
    std::vector<Enemy> enemies_;
    std::list<std::string> occupantLog_;
};
