#pragma once

#include <SFML/Graphics/Color.hpp>

enum class Biome { Meadow, Forest, Swamp, Wasteland, Cavern };

struct BiomeProfile {
    const char* name;
    sf::Color ground;
    sf::Color outline;
    float enemyChanceMult;
    float nectarValueMult;
    float obstacleChanceMult;
    int enemyTierBonus;
    float playerSpeedMult;
};

Biome biomeForChunk(int chunkX, int chunkY, unsigned int seed);
const BiomeProfile& biomeProfile(Biome biome);
