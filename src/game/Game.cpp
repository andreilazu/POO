#include "game/Game.hpp"

#include "core/ResourceLoadException.hpp"
#include "game/EntityFactory.hpp"
#include "game/EntityInspector.hpp"
#include "game/GameOverState.hpp"
#include "game/PlayState.hpp"
#include "world/Biome.hpp"
#include "entities/Creature.hpp"
#include "entities/Enemy.hpp"
#include "world/WorldChunk.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/View.hpp>

#include <cmath>
#include <iostream>
#include <optional>
#include <string>

Game::Game()
    : player_(EntityFactory::createPlayer({400.f, 300.f})),
      swarm_(),
      worldSeed_(ProceduralGenerator::seedFromTime()),
      world_(worldSeed_),
      state_(std::make_unique<PlayState>()),
      spawnPosition_({400.f, 300.f}) {
    events_.subscribe("swarm.grew", [](const std::string& payload) {
        std::cout << "[Event] swarm grew: " << payload << '\n';
    });
    events_.subscribe("creature.collected", [](const std::string& payload) {
        std::cout << "[Collect] " << payload << '\n';
    });
    events_.subscribe("enemy.defeated", [](const std::string& payload) {
        std::cout << "[Combat] " << payload << '\n';
    });

    const CreatureType starters[] = {CreatureType::Red, CreatureType::Yellow,
                                     CreatureType::Blue};
    for (CreatureType type : starters) {
        auto creature = EntityFactory::createCreature(type, player_->position());
        swarm_.addCreature(*creature);
    }
    events_.publish("swarm.grew", std::to_string(swarm_.creatures().size()));

    world_.ensureLoadedAround(player_->position());
}

int Game::run() {
    try {
        sf::RenderWindow window(sf::VideoMode({960U, 540U}), "Pikmin Swarm");
        window.setFramerateLimit(60);

        sf::Clock clock;
        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                        window.close();
                    }
                }
            }

            if (state_) {
                state_->handleInput(*this, window);
            }

            const sf::Time deltaTime = clock.restart();
            update(deltaTime);
            render(window);
        }
        return 0;
    } catch (const ResourceLoadException& ex) {
        std::cerr << "Resource error: " << ex.what() << '\n';
        return 1;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
}

void Game::setState(std::unique_ptr<GameState> state) {
    state_ = std::move(state);
}

Player& Game::player() {
    return *player_;
}

const Player& Game::player() const {
    return *player_;
}

SwarmManager& Game::swarm() {
    return swarm_;
}

ProceduralGenerator& Game::world() {
    return world_;
}

EventBus& Game::events() {
    return events_;
}

void Game::update(sf::Time deltaTime) {
    if (state_) {
        state_->update(*this, deltaTime);
    }
}

void Game::updateWorld() {
    world_.ensureLoadedAround(player_->position());
}

void Game::movePlayer(sf::Vector2f direction, sf::Time deltaTime) {
    if (direction.x == 0.f && direction.y == 0.f) {
        return;
    }

    const BiomeProfile& biome = biomeProfile(world_.biomeAt(player_->position()));
    direction *= biome.playerSpeedMult;

    const sf::Vector2f previousPosition = player_->position();
    player_->move(direction, deltaTime);
    if (world_.blocksCircle(player_->position(), Player::radius())) {
        player_->setPosition(previousPosition);
    }
}

void Game::processWorldInteractions() {
    const float playerRadius = EntityInspector::gatherRadiusFor(*player_);
    const int fromPlayer = world_.collectAt(player_->position(), playerRadius);
    if (fromPlayer > 0) {
        player_->addNectar(fromPlayer);
        events_.publish("creature.collected", "nectar x" + std::to_string(fromPlayer));
    }

    for (const auto& creature : swarm_.creatures()) {
        const float gatherRadius = EntityInspector::gatherRadiusFor(*creature);
        const int fromSwarm = world_.collectAt(creature->position(), gatherRadius);
        if (fromSwarm > 0) {
            player_->addNectar(fromSwarm);
            events_.publish("creature.collected",
                             EntityInspector::describeForHud(*creature) + " +" +
                                 std::to_string(fromSwarm));
        }
    }

    tryGrowSwarmFromNectar();
}

void Game::updateDifficulty() {
    const sf::Vector2f delta = player_->position() - spawnPosition_;
    difficulty_ = std::sqrt(delta.x * delta.x + delta.y * delta.y) * 0.08f;
    world_.setDifficulty(difficulty_);
}

int Game::swarmGrowCost() const {
    const int swarmSize = static_cast<int>(swarm_.creatures().size());
    return 5 + swarmSize / 2;
}

void Game::tryGrowSwarmFromNectar() {
    if (player_->nectarCollected() < swarmGrowCost()) {
        return;
    }
    const int cost = swarmGrowCost();
    if (!player_->spendNectar(cost)) {
        return;
    }
    const int n = static_cast<int>(swarm_.creatures().size());
    const CreatureType types[] = {CreatureType::Red, CreatureType::Yellow, CreatureType::Blue};
    auto creature = EntityFactory::createCreature(types[n % 3], player_->position());
    swarm_.addCreature(*creature);
    events_.publish("swarm.grew", std::to_string(swarm_.creatures().size()));
}

sf::Vector2f Game::spawnPosition() const {
    return spawnPosition_;
}

float Game::difficulty() const {
    return difficulty_;
}

bool Game::isGameOver() const {
    return dynamic_cast<GameOverState*>(state_.get()) != nullptr;
}

void Game::checkPlayerDeath() {
    if (!player_->isAlive() && !isGameOver()) {
        setState(std::make_unique<GameOverState>());
    }
}

void Game::restartRun() {
    player_ = EntityFactory::createPlayer(spawnPosition_);
    swarm_ = SwarmManager{};
    world_ = ProceduralGenerator{worldSeed_};
    difficulty_ = 0.f;

    const CreatureType starters[] = {CreatureType::Red, CreatureType::Yellow,
                                     CreatureType::Blue};
    for (CreatureType type : starters) {
        auto creature = EntityFactory::createCreature(type, player_->position());
        swarm_.addCreature(*creature);
    }

    world_.ensureLoadedAround(player_->position());
    setState(std::make_unique<PlayState>());
}

void Game::renderWorld(sf::RenderTarget& target) const {
    for (const auto& entry : world_.chunks()) {
        const ChunkId& id = entry.first;
        const BiomeProfile& profile = biomeProfile(entry.second.biome());

        sf::RectangleShape tile({ProceduralGenerator::kChunkWorldSize,
                                 ProceduralGenerator::kChunkWorldSize});
        tile.setPosition({id.x * ProceduralGenerator::kChunkWorldSize,
                          id.y * ProceduralGenerator::kChunkWorldSize});
        tile.setFillColor(profile.ground);
        tile.setOutlineColor(profile.outline);
        tile.setOutlineThickness(1.f);
        target.draw(tile);
    }

    for (const auto& entry : world_.chunks()) {
        const WorldChunk& chunk = entry.second;

        for (const auto& collectible : chunk.collectibles()) {
            sf::CircleShape marker(7.f);
            marker.setFillColor(sf::Color(240, 200, 60));
            marker.setPosition({collectible.position().x - 7.f, collectible.position().y - 7.f});
            target.draw(marker);
        }

        for (const auto& obstacle : chunk.obstacles()) {
            if (obstacle.isDestroyed()) {
                continue;
            }
            const float healthRatio =
                static_cast<float>(obstacle.health()) / static_cast<float>(obstacle.maxHealth());
            const std::uint8_t channel =
                static_cast<std::uint8_t>(90.f + healthRatio * 50.f);
            sf::CircleShape rock(obstacle.radius());
            rock.setFillColor(sf::Color(channel, channel, static_cast<std::uint8_t>(130 + healthRatio * 30.f)));
            rock.setPosition({obstacle.position().x - obstacle.radius(),
                              obstacle.position().y - obstacle.radius()});
            target.draw(rock);
        }

        for (const Enemy& enemy : chunk.enemies()) {
            if (enemy.isAlive()) {
                enemy.draw(target);
            }
        }
    }
}

void Game::renderScene(sf::RenderTarget& target) const {
    const sf::Vector2f center = player_->position();
    sf::View worldView;
    worldView.setSize({960.f, 540.f});
    worldView.setCenter(center);
    target.setView(worldView);

    renderWorld(target);
    swarm_.renderBlob(target, player_->position());
    if (player_->isAlive()) {
        player_->draw(target);
    }
    for (const auto& creature : swarm_.creatures()) {
        if (creature->isAlive()) {
            creature->draw(target);
        }
    }

    target.setView(target.getDefaultView());
}

void Game::render(sf::RenderWindow& window) {
    window.clear(sf::Color(30, 40, 55));
    if (state_) {
        state_->render(*this, window);
    }
    window.display();
}
