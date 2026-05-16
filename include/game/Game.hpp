#pragma once

#include "entities/Player.hpp"
#include "game/EventBus.hpp"
#include "game/GameState.hpp"
#include "swarm/SwarmManager.hpp"
#include "world/ProceduralGenerator.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

class Game {
public:
    Game();

    int run();
    void setState(std::unique_ptr<GameState> state);

    Player& player();
    const Player& player() const;
    SwarmManager& swarm();
    ProceduralGenerator& world();
    EventBus& events();

    void updateWorld();
    void processWorldInteractions();
    void tryGrowSwarmFromNectar();
    void updateDifficulty();
    void movePlayer(sf::Vector2f direction, sf::Time deltaTime);
    void renderWorld(sf::RenderTarget& target) const;
    void renderScene(sf::RenderTarget& target) const;
    void checkPlayerDeath();
    void restartRun();

    sf::Vector2f spawnPosition() const;
    float difficulty() const;
    int swarmGrowCost() const;
    bool isGameOver() const;

private:
    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);

    std::unique_ptr<Player> player_;
    sf::Vector2f spawnPosition_;
    float difficulty_ = 0.f;
    unsigned int worldSeed_ = 0U;
    SwarmManager swarm_;
    ProceduralGenerator world_;
    EventBus events_;
    std::unique_ptr<GameState> state_;
};
