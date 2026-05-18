#pragma once

#include "world/Biome.hpp"
#include "world/WorldChunk.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <optional>
#include <unordered_map>

class ProceduralGenerator {
public:
    static unsigned int seed();
    static unsigned int seedFromTime();

    explicit ProceduralGenerator(unsigned int seedValue);

    WorldChunk& chunkAt(int x, int y);
    void ensureLoadedAround(sf::Vector2f worldPosition, int radiusChunks = 2);
    void setDifficulty(float difficulty);
    void updateEnemies(sf::Vector2f playerPosition, sf::Time deltaTime);
    void processEnemyAttacks(class Player& player, const class SwarmManager& swarm, sf::Time deltaTime);
    int processCombatWithSwarm(class SwarmManager& swarm);
    Biome biomeAt(sf::Vector2f worldPosition) const;
    bool blocksCircle(sf::Vector2f center, float radius) const;
    int collectAt(sf::Vector2f center, float radius);
    std::optional<sf::Vector2f> nearestCollectible(sf::Vector2f from, float maxDistance) const;
    int damageObstaclesNear(sf::Vector2f center, float radius, int damage);
    const std::unordered_map<ChunkId, WorldChunk, ChunkIdHash>& chunks() const;

    static constexpr float kChunkWorldSize = 64.f;

private:
    unsigned int seed_;
    float difficulty_ = 0.f;
    std::unordered_map<ChunkId, WorldChunk, ChunkIdHash> chunks_;
};
