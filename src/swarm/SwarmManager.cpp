#include "swarm/SwarmManager.hpp"

#include "entities/Creature.hpp"
#include "entities/Player.hpp"

#include <SFML/Graphics/CircleShape.hpp>

#include <algorithm>
#include <cmath>
#include <sstream>
#include <unordered_set>
#include <vector>

int SwarmManager::totalSpawned_ = 0;

SwarmManager::SwarmManager() : creatures_(), taskLog_() {}

void SwarmManager::addCreature(Creature creature) {
    const std::size_t index = creatures_.size();
    creature.initSwarmTraits(index);
    creatures_.push_back(std::make_unique<Creature>(std::move(creature)));
    ++totalSpawned_;
}

void SwarmManager::assignTask(Entity& entity, SwarmTask task) {
    std::ostringstream label;
    label << entity.typeName() << " -> ";
    switch (task) {
    case SwarmTask::Follow:
        label << "Follow";
        break;
    case SwarmTask::Gather:
        label << "Gather";
        break;
    case SwarmTask::Attack:
        label << "Attack";
        break;
    }
    taskLog_.push_back(label.str());

    if (auto* creature = dynamic_cast<Creature*>(&entity)) {
        creature->setPosition(entity.position());
        (void)creature;
    }
}

void SwarmManager::sortByDistanceTo(const Player& player) {
    const sf::Vector2f origin = player.position();
    std::sort(creatures_.begin(), creatures_.end(),
              [&origin](const std::unique_ptr<Creature>& a, const std::unique_ptr<Creature>& b) {
                  const auto da = (a->position() - origin);
                  const auto db = (b->position() - origin);
                  const float distA = da.x * da.x + da.y * da.y;
                  const float distB = db.x * db.x + db.y * db.y;
                  return distA < distB;
              });
}

namespace {

constexpr float kCreatureRadius = 6.f;
constexpr float kMinCreatureSpacing = 14.f;
constexpr float kBaseFollowSpeed = 200.f;
constexpr float kGatherSearchRadius = 165.f;
constexpr float kPi = 3.14159265f;

float length(sf::Vector2f v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f normalized(sf::Vector2f v) {
    const float len = length(v);
    if (len < 0.0001f) {
        return {1.f, 0.f};
    }
    return {v.x / len, v.y / len};
}

void moveToward(Creature& creature, sf::Vector2f target, float speed, float dt) {
    sf::Vector2f pos = creature.position();
    const sf::Vector2f toTarget = target - pos;
    const float dist = length(toTarget);
    if (dist > 0.001f) {
        const float step = speed * dt;
        pos += (step >= dist) ? toTarget : (toTarget / dist) * step;
    } else {
        pos = target;
    }
    creature.setPosition(pos);
}

std::vector<sf::Vector2f> formationTargets(sf::Vector2f anchor,
                                           const std::vector<std::unique_ptr<Creature>>& creatures) {
    const std::size_t count = creatures.size();
    std::vector<sf::Vector2f> targets;
    if (count == 0) {
        return targets;
    }

    const float minOrbit = Player::kRadius + kCreatureRadius + 8.f;
    targets.reserve(count);

    std::size_t assigned = 0;
    int ring = 0;
    while (assigned < count) {
        const float radius = minOrbit + static_cast<float>(ring) * kMinCreatureSpacing;
        const float circumference = 2.f * kPi * radius;
        const std::size_t slots =
            std::max<std::size_t>(1U, static_cast<std::size_t>(circumference / kMinCreatureSpacing));
        const std::size_t ringCount = std::min(slots, count - assigned);
        for (std::size_t slot = 0; slot < ringCount; ++slot) {
            float angle = (2.f * kPi * static_cast<float>(slot)) / static_cast<float>(ringCount);
            angle += creatures[assigned]->phaseOffset();
            targets.push_back(
                anchor + sf::Vector2f{std::cos(angle) * radius, std::sin(angle) * radius});
            ++assigned;
        }
        ++ring;
    }
    return targets;
}

void separateCreatures(std::vector<std::unique_ptr<Creature>>& creatures, sf::Vector2f anchor) {
    const float minFromPlayer = Player::kRadius + kCreatureRadius + 2.f;
    const float minFromPlayerSq = minFromPlayer * minFromPlayer;
    const float minBetweenSq = kMinCreatureSpacing * kMinCreatureSpacing;

    for (int pass = 0; pass < 2; ++pass) {
        for (auto& creature : creatures) {
            sf::Vector2f pos = creature->position();

            sf::Vector2f fromPlayer = pos - anchor;
            const float playerDistSq = fromPlayer.x * fromPlayer.x + fromPlayer.y * fromPlayer.y;
            if (playerDistSq < minFromPlayerSq) {
                pos = anchor + normalized(fromPlayer) * minFromPlayer;
            }

            creature->setPosition(pos);
        }

        for (std::size_t i = 0; i < creatures.size(); ++i) {
            sf::Vector2f posI = creatures[i]->position();
            for (std::size_t j = i + 1; j < creatures.size(); ++j) {
                sf::Vector2f posJ = creatures[j]->position();
                sf::Vector2f delta = posI - posJ;
                const float distSq = delta.x * delta.x + delta.y * delta.y;
                if (distSq >= minBetweenSq || distSq < 0.0001f) {
                    continue;
                }
                const float dist = std::sqrt(distSq);
                const float push = (kMinCreatureSpacing - dist) * 0.5f;
                const sf::Vector2f n = {delta.x / dist, delta.y / dist};
                posI += n * push;
                posJ -= n * push;
                creatures[i]->setPosition(posI);
                creatures[j]->setPosition(posJ);
            }
        }
    }
}

void applyOverlapAlpha(std::vector<std::unique_ptr<Creature>>& creatures) {
    for (auto& creature : creatures) {
        int neighbors = 0;
        for (const auto& other : creatures) {
            if (other.get() == creature.get()) {
                continue;
            }
            const sf::Vector2f delta = creature->position() - other->position();
            if ((delta.x * delta.x + delta.y * delta.y) < (kMinCreatureSpacing * kMinCreatureSpacing)) {
                ++neighbors;
            }
        }
        const float alpha = 255.f - static_cast<float>(std::min(neighbors, 4)) * 28.f;
        creature->setDrawAlpha(alpha);
    }
}

std::unordered_set<std::size_t> pickGatherers(
    const std::vector<std::unique_ptr<Creature>>& creatures, sf::Vector2f gatherTarget) {
    std::unordered_set<std::size_t> gatherers;
    const std::size_t count = creatures.size();
    if (count == 0) {
        return gatherers;
    }

    const std::size_t gatherCount = std::max<std::size_t>(2U, count / 5U);
    std::vector<std::pair<float, std::size_t>> ranked;
    ranked.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        const sf::Vector2f delta = creatures[i]->position() - gatherTarget;
        float score = delta.x * delta.x + delta.y * delta.y;
        if (creatures[i]->creatureType() == CreatureType::Yellow) {
            score *= 0.6f;
        }
        ranked.emplace_back(score, i);
    }
    std::sort(ranked.begin(), ranked.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });

    for (std::size_t n = 0; n < std::min(gatherCount, count); ++n) {
        gatherers.insert(ranked[n].second);
    }
    return gatherers;
}

int obstacleDamageFor(const Creature& creature) {
    switch (creature.creatureType()) {
    case CreatureType::Red:
        return 2;
    case CreatureType::Blue:
        return 1;
    case CreatureType::Yellow:
        return 1;
    }
    return 1;
}

}  // namespace

void SwarmManager::update(sf::Time deltaTime, const Player& player, ProceduralGenerator& world) {
    const sf::Vector2f anchor = player.position();
    const float dt = deltaTime.asSeconds();
    const std::size_t swarmSize = creatures_.size();
    const float swarmSpeedBoost = std::min(55.f, static_cast<float>(swarmSize) * 0.9f);

    const std::optional<sf::Vector2f> gatherTarget =
        world.nearestCollectible(anchor, kGatherSearchRadius);
    const std::unordered_set<std::size_t> gatherers =
        gatherTarget.has_value() ? pickGatherers(creatures_, *gatherTarget) : std::unordered_set<std::size_t>{};

    const std::vector<sf::Vector2f> targets = formationTargets(anchor, creatures_);

    for (std::size_t i = 0; i < creatures_.size(); ++i) {
        auto& creature = creatures_[i];
        const bool isGathering = gatherTarget.has_value() && gatherers.count(i) > 0;
        const float speedScale = 1.f - creature->followLag();
        float speed = creature->moveSpeed() * speedScale + swarmSpeedBoost;

        if (isGathering) {
            speed *= 1.15f;
            moveToward(*creature, *gatherTarget, speed, dt);
        } else {
            const float followSpeed = kBaseFollowSpeed * speedScale + swarmSpeedBoost * 0.5f;
            moveToward(*creature, targets[i], followSpeed, dt);
        }

        creature->update(deltaTime);

        const int chipDamage = obstacleDamageFor(*creature);
        const float chipRadius = creature->creatureType() == CreatureType::Red ? 14.f : 10.f;
        world.damageObstaclesNear(creature->position(), chipRadius, chipDamage);
    }

    separateCreatures(creatures_, anchor);
    applyOverlapAlpha(creatures_);
}

void SwarmManager::renderBlob(sf::RenderTarget& target, sf::Vector2f anchor) const {
    if (creatures_.empty()) {
        return;
    }
    const float radius =
        Player::kRadius + 10.f + std::sqrt(static_cast<float>(creatures_.size())) * 6.f;
    sf::CircleShape blob(radius);
    blob.setOrigin({radius, radius});
    blob.setPosition(anchor);
    blob.setFillColor(sf::Color(90, 180, 120, 28));
    blob.setOutlineColor(sf::Color(120, 200, 140, 45));
    blob.setOutlineThickness(2.f);
    target.draw(blob);
}

const std::vector<std::unique_ptr<Creature>>& SwarmManager::creatures() const {
    return creatures_;
}

void SwarmManager::removeDeadCreatures() {
    creatures_.erase(std::remove_if(creatures_.begin(), creatures_.end(),
                                    [](const std::unique_ptr<Creature>& creature) {
                                        return !creature->isAlive();
                                    }),
                     creatures_.end());
}

int SwarmManager::totalCreaturesSpawned() {
    return totalSpawned_;
}
