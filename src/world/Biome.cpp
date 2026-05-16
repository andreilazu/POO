#include "world/Biome.hpp"

#include <functional>

namespace {

constexpr int kBiomeRegionChunks = 5;

}  // namespace

Biome biomeForChunk(int chunkX, int chunkY, unsigned int seed) {
    const int regionX = chunkX / kBiomeRegionChunks;
    const int regionY = chunkY / kBiomeRegionChunks;
    const auto hash = static_cast<unsigned int>(
        std::hash<int>{}(regionX) ^
        (std::hash<int>{}(regionY + static_cast<int>(seed)) * 2654435761U));
    return static_cast<Biome>(hash % 5U);
}

const BiomeProfile& biomeProfile(Biome biome) {
    static const BiomeProfile kProfiles[] = {
        {"Meadow",
         sf::Color(42, 72, 48),
         sf::Color(58, 96, 64),
         1.0f,
         1.0f,
         1.0f,
         0,
         1.0f},
        {"Forest",
         sf::Color(28, 58, 36),
         sf::Color(40, 78, 50),
         0.75f,
         1.35f,
         1.4f,
         0,
         0.95f},
        {"Swamp",
         sf::Color(34, 52, 44),
         sf::Color(48, 70, 58),
         1.08f,
         0.9f,
         1.15f,
         1,
         0.92f},
        {"Wasteland",
         sf::Color(62, 48, 38),
         sf::Color(82, 64, 50),
         1.35f,
         0.75f,
         0.65f,
         1,
         1.0f},
        {"Cavern",
         sf::Color(36, 40, 58),
         sf::Color(52, 58, 78),
         1.25f,
         1.5f,
         1.8f,
         1,
         0.92f},
    };
    return kProfiles[static_cast<int>(biome)];
}
