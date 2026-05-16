#include "world/WorldChunk.hpp"

#include "entities/Creature.hpp"
#include "entities/Enemy.hpp"
#include "entities/Player.hpp"

bool ChunkId::operator==(const ChunkId& other) const {
    return x == other.x && y == other.y;
}

std::size_t ChunkIdHash::operator()(const ChunkId& id) const noexcept {
    const auto hx = static_cast<std::size_t>(id.x);
    const auto hy = static_cast<std::size_t>(id.y);
    return hx ^ (hy << 1U);
}

WorldChunk::WorldChunk(ChunkId id, Biome biome) : id_(id), biome_(biome) {}

Biome WorldChunk::biome() const {
    return biome_;
}

void WorldChunk::addCollectible(Collectible item) {
    collectibles_.push_back(std::move(item));
}

void WorldChunk::addObstacle(Obstacle item) {
    obstacles_.push_back(std::move(item));
}

void WorldChunk::addEnemy(Enemy enemy) {
    enemies_.emplace_back(std::move(enemy));
}

void WorldChunk::registerOccupant(const Entity& entity) {
    occupantLog_.push_back(entity.typeName());
}

void WorldChunk::tickOccupants() {
    for (const auto& name : occupantLog_) {
        if (name == std::string("Creature")) {
            continue;
        }
    }
}

const ChunkId& WorldChunk::id() const {
    return id_;
}

int WorldChunk::collectAt(sf::Vector2f center, float radius) {
    const float radiusSq = radius * radius;
    int gained = 0;
    auto it = collectibles_.begin();
    while (it != collectibles_.end()) {
        const sf::Vector2f delta = it->position() - center;
        if ((delta.x * delta.x + delta.y * delta.y) <= radiusSq) {
            gained += it->value();
            it = collectibles_.erase(it);
        } else {
            ++it;
        }
    }
    return gained;
}

int WorldChunk::damageObstaclesAt(sf::Vector2f center, float radius, int damage) {
    const float radiusSq = radius * radius;
    int nectarDropped = 0;
    auto it = obstacles_.begin();
    while (it != obstacles_.end()) {
        const sf::Vector2f delta = it->position() - center;
        if ((delta.x * delta.x + delta.y * delta.y) > radiusSq || it->isDestroyed()) {
            ++it;
            continue;
        }
        it->takeDamage(damage);
        if (it->isDestroyed()) {
            const int reward = 1 + static_cast<int>(it->radius() / 6.f);
            collectibles_.emplace_back(it->position(), reward);
            nectarDropped += reward;
            it = obstacles_.erase(it);
        } else {
            ++it;
        }
    }
    return nectarDropped;
}

const std::vector<Collectible>& WorldChunk::collectibles() const {
    return collectibles_;
}

const std::vector<Obstacle>& WorldChunk::obstacles() const {
    return obstacles_;
}

std::vector<Enemy>& WorldChunk::enemies() {
    return enemies_;
}

const std::vector<Enemy>& WorldChunk::enemies() const {
    return enemies_;
}
