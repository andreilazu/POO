#include "world/ProceduralGenerator.hpp"

#include "entities/Enemy.hpp"
#include "entities/Player.hpp"
#include "swarm/CombatSystem.hpp"
#include "swarm/SwarmManager.hpp"
#include "world/Biome.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>

unsigned int ProceduralGenerator::seed() {
    return 1337U;
}

unsigned int ProceduralGenerator::seedFromTime() {
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    return static_cast<unsigned int>(now & 0xFFFFFFFFU);
}

ProceduralGenerator::ProceduralGenerator(unsigned int seedValue) : seed_(seedValue) {}

WorldChunk& ProceduralGenerator::chunkAt(int x, int y) {
    const ChunkId id{x, y};
    auto found = chunks_.find(id);
    if (found != chunks_.end()) {
        return found->second;
    }

    const auto hash = std::hash<int>{}(x) ^ (std::hash<int>{}(y) + seed_);
    const Biome biome = biomeForChunk(x, y, seed_);
    const BiomeProfile& profile = biomeProfile(biome);
    WorldChunk chunk(id, biome);
    const sf::Vector2f chunkOrigin{x * kChunkWorldSize, y * kChunkWorldSize};
    const int nectarBonus = static_cast<int>(difficulty_ * 0.1f * profile.nectarValueMult);
    const unsigned int nectarMod = profile.nectarValueMult >= 1.2f ? 4U : 5U;
    if ((hash % nectarMod) == 0U) {
        const int value = static_cast<int>((1 + static_cast<int>(hash % 3U) + nectarBonus) *
                                           profile.nectarValueMult);
        chunk.addCollectible(
            Collectible({chunkOrigin.x + 28.f, chunkOrigin.y + 28.f}, std::max(1, value)));
    }
    const unsigned int obstacleMod =
        profile.obstacleChanceMult >= 1.5f ? 5U : (profile.obstacleChanceMult >= 1.2f ? 6U : 7U);
    if ((hash % obstacleMod) == 0U) {
        const float rockSize = profile.obstacleChanceMult >= 1.5f ? 14.f : 12.f;
        chunk.addObstacle(Obstacle({chunkOrigin.x + 44.f, chunkOrigin.y + 44.f}, rockSize));
    }

    const unsigned int enemyRoll = (hash >> 4U) % 100U;
    const float enemyChance =
        std::min(52.f, (10.f + difficulty_ * 0.33f) * profile.enemyChanceMult);
    if (enemyRoll < static_cast<unsigned int>(enemyChance)) {
        const int tier =
            std::clamp(1 + static_cast<int>(difficulty_ * 0.02f) + profile.enemyTierBonus, 1, 10);
        const float offsetX = 12.f + static_cast<float>((hash >> 8U) % 40U);
        const float offsetY = 12.f + static_cast<float>((hash >> 12U) % 40U);
        chunk.addEnemy(Enemy({chunkOrigin.x + offsetX, chunkOrigin.y + offsetY}, tier));
    }
    if ((hash % 11U) == 0U && difficulty_ > 22.f) {
        const int tier =
            std::clamp(1 + static_cast<int>(difficulty_ * 0.03f) + profile.enemyTierBonus, 1, 12);
        chunk.addEnemy(Enemy({chunkOrigin.x + 50.f, chunkOrigin.y + 18.f}, tier));
    }

    const auto inserted = chunks_.emplace(id, std::move(chunk));
    return inserted.first->second;
}

void ProceduralGenerator::ensureLoadedAround(sf::Vector2f worldPosition, int radiusChunks) {
    const int centerX =
        static_cast<int>(std::floor(worldPosition.x / kChunkWorldSize));
    const int centerY =
        static_cast<int>(std::floor(worldPosition.y / kChunkWorldSize));
    for (int dy = -radiusChunks; dy <= radiusChunks; ++dy) {
        for (int dx = -radiusChunks; dx <= radiusChunks; ++dx) {
            chunkAt(centerX + dx, centerY + dy);
        }
    }
}

bool ProceduralGenerator::blocksCircle(sf::Vector2f center, float radius) const {
    for (const auto& entry : chunks_) {
        for (const Obstacle& obstacle : entry.second.obstacles()) {
            if (obstacle.isDestroyed()) {
                continue;
            }
            const sf::Vector2f delta = center - obstacle.position();
            const float minDistance = radius + obstacle.radius();
            if ((delta.x * delta.x + delta.y * delta.y) < (minDistance * minDistance)) {
                return true;
            }
        }
    }
    return false;
}

int ProceduralGenerator::collectAt(sf::Vector2f center, float radius) {
    int gained = 0;
    for (auto& entry : chunks_) {
        gained += entry.second.collectAt(center, radius);
    }
    return gained;
}

const std::unordered_map<ChunkId, WorldChunk, ChunkIdHash>& ProceduralGenerator::chunks() const {
    return chunks_;
}

std::optional<sf::Vector2f> ProceduralGenerator::nearestCollectible(sf::Vector2f from,
                                                                    float maxDistance) const {
    const float maxDistSq = maxDistance * maxDistance;
    std::optional<sf::Vector2f> nearest;
    float bestDistSq = maxDistSq;

    for (const auto& entry : chunks_) {
        for (const Collectible& item : entry.second.collectibles()) {
            const sf::Vector2f delta = item.position() - from;
            const float distSq = delta.x * delta.x + delta.y * delta.y;
            if (distSq < bestDistSq) {
                bestDistSq = distSq;
                nearest = item.position();
            }
        }
    }
    return nearest;
}

int ProceduralGenerator::damageObstaclesNear(sf::Vector2f center, float radius, int damage) {
    int total = 0;
    for (auto& entry : chunks_) {
        total += entry.second.damageObstaclesAt(center, radius, damage);
    }
    return total;
}

void ProceduralGenerator::setDifficulty(float difficulty) {
    difficulty_ = std::max(0.f, difficulty);
}

void ProceduralGenerator::updateEnemies(sf::Vector2f playerPosition, sf::Time deltaTime) {
    for (auto& entry : chunks_) {
        for (Enemy& enemy : entry.second.enemies()) {
            if (enemy.isAlive()) {
                enemy.chase(playerPosition, deltaTime);
                enemy.update(deltaTime);
            }
        }
    }
}

void ProceduralGenerator::processEnemyAttacks(Player& player, SwarmManager& swarm,
                                              sf::Time deltaTime) {
    for (auto& entry : chunks_) {
        for (Enemy& enemy : entry.second.enemies()) {
            if (!enemy.isAlive()) {
                continue;
            }

            const float attackRange = Enemy::kAttackRange + Enemy::kRadius;
            const float attackRangeSq = attackRange * attackRange;
            const sf::Vector2f playerDelta = player.position() - enemy.position();
            const bool playerInRange =
                (playerDelta.x * playerDelta.x + playerDelta.y * playerDelta.y) <= attackRangeSq;

            if (player.isAlive() && player.canTakeDamage() && playerInRange) {
                if (enemy.tryAttack(player, deltaTime)) {
                    player.onDamaged();
                }
                continue;
            }

            Creature* nearestCreature = nullptr;
            float nearestDistSq = attackRangeSq;
            for (const auto& creature : swarm.creatures()) {
                if (!creature->isAlive()) {
                    continue;
                }
                const sf::Vector2f delta = creature->position() - enemy.position();
                const float distSq = delta.x * delta.x + delta.y * delta.y;
                if (distSq <= nearestDistSq) {
                    nearestDistSq = distSq;
                    nearestCreature = creature.get();
                }
            }
            if (nearestCreature != nullptr) {
                enemy.tryAttack(*nearestCreature, deltaTime);
            }
        }
    }
}

Biome ProceduralGenerator::biomeAt(sf::Vector2f worldPosition) const {
    const int chunkX =
        static_cast<int>(std::floor(worldPosition.x / kChunkWorldSize));
    const int chunkY =
        static_cast<int>(std::floor(worldPosition.y / kChunkWorldSize));
    const ChunkId id{chunkX, chunkY};
    const auto found = chunks_.find(id);
    if (found != chunks_.end()) {
        return found->second.biome();
    }
    return biomeForChunk(chunkX, chunkY, seed_);
}

int ProceduralGenerator::processCombatWithSwarm(SwarmManager& swarm) {
    constexpr float kCombatRadius = 30.f;
    const float combatRadiusSq = kCombatRadius * kCombatRadius;
    int nectarDropped = 0;

    for (auto& entry : chunks_) {
        WorldChunk& chunk = entry.second;
        auto& enemies = chunk.enemies();
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
            Enemy& enemy = *enemyIt;
            if (!enemy.isAlive()) {
                const int reward = 2 + enemy.tier();
                chunk.addCollectible(Collectible(enemy.position(), reward));
                nectarDropped += reward;
                enemyIt = enemies.erase(enemyIt);
                continue;
            }

            for (const auto& creature : swarm.creatures()) {
                const sf::Vector2f delta = creature->position() - enemy.position();
                if ((delta.x * delta.x + delta.y * delta.y) <= combatRadiusSq) {
                    CombatSystem::resolve(*creature, enemy);
                }
            }

            if (!enemy.isAlive()) {
                const int reward = 2 + enemy.tier();
                chunk.addCollectible(Collectible(enemy.position(), reward));
                nectarDropped += reward;
                enemyIt = enemies.erase(enemyIt);
            } else {
                ++enemyIt;
            }
        }
    }
    return nectarDropped;
}
